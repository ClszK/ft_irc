#include "server/Server.hpp"

#include "channel/Channel.hpp"
#include "client/Client.hpp"
#include "command/CommandHandler.hpp"

void Server::setChannel(const std::string& channelName, Channel* channel) {
  if (channel == NULL) {
    delete mChannels[channelName];
    mChannels.erase(channelName);
  } else
    mChannels[channelName] = channel;
}

void Server::setClient(const int sockFd, Client* client) {
  if (client == NULL) {
    delete mClients[sockFd];
    mClients.erase(sockFd);
  } else
    mClients[sockFd] = client;
}

void Server::init() {
  NumericReply::initializeReplies();
  if ((mListenFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    throw std::runtime_error(std::strerror(errno));

  mServerAddr = SocketAddr(mServerConf.port);
  int yes = 1;
  if (setsockopt(mListenFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    throw std::runtime_error(std::strerror(errno));

  if (bind(mListenFd, mServerAddr, sizeof(struct sockaddr)) == -1 ||
      listen(mListenFd, MAX_EVENTS) == -1 || (mKq = kqueue()) == -1)
    throw std::runtime_error(std::strerror(errno));

  struct kevent servEvent;
  EV_SET(&servEvent, mListenFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
  kevent(mKq, &servEvent, 1, NULL, 0, NULL);
}

void Server::initServerInfo(char* argv[]) {
  char* end;
  long port = std::strtol(argv[1], &end, 10);
  if (errno == ERANGE || *end != '\0' || port < 0 || port > 65535)
    throw std::runtime_error("Invalid port number");

  mServerConf.port = port;
  char hostBuffer[256];
  if (gethostname(hostBuffer, sizeof(hostBuffer)) == -1)
    throw std::runtime_error(std::strerror(errno));

  std::time_t rawTime;
  std::tm* timeInfo;
  char buffer[80];

  std::time(&rawTime);
  timeInfo = std::localtime(&rawTime);

  std::strftime(buffer, sizeof(buffer), "%H:%M:%S %b %d %Y", timeInfo);

  mServerConf.password = argv[2];
  mServerConf.hostName = std::string(hostBuffer);
  mServerConf.version = "ircserv-1.0";
  mServerConf.serverName = "ft_ircd";
  mServerConf.createdTime = std::string(buffer);
  /**
   * i: 클라이언트가 다른 사용자에게 보이지 않도록 하여 프라이버시를 보호.
   * o: 운영자가 서버를 관리하고, 문제를 해결할 수 있는 권한 부여.
   * s: 서버의 중요한 이벤트나 메시지를 운영자에게 전달.
   * w: 서버 전체에 중요한 메시지를 브로드캐스트하기 위해 사용.
   */
  mServerConf.availableUserMode = "iosw";
  /**
   * i: invite only
   * t: topic settable by channel operator only
   * k: channel key required to join
   * l: limit on number of users that can join
   * o: operator
   */
  mServerConf.availableChannelMode = "itknol";
}

void Server::handleListenEvent() {
  int connFd;
  SocketAddr clientAddr;
  socklen_t sin_size;

  if ((connFd = accept(mListenFd, clientAddr, &sin_size)) == -1 ||
      fcntl(connFd, F_SETFL, O_NONBLOCK) == -1)
    throw std::runtime_error(std::strerror(errno));

  struct kevent listenEvent;
  EV_SET(&listenEvent, connFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
  kevent(mKq, &listenEvent, 1, NULL, 0, NULL);

  Client::createClient(connFd, inet_ntoa(clientAddr.getAddr()->sin_addr));
}

/**
 * TODO : irssi에서 종료할 때 connection close가 무한 반복함.
 * 나중에 고쳐야함.
 */
void Server::handleReadEvent(struct kevent& event) {
  char buffer[20000];

  int n = recv(event.ident, buffer, sizeof(buffer) - 1, 0);
  if (n > 0) {
    buffer[n] = '\0';
    mBuffers[event.ident] += buffer;

    if (std::find(mBuffers[event.ident].begin(), mBuffers[event.ident].end(),
                  '\n') != mBuffers[event.ident].end()) {
      struct kevent writeEvent;
      EV_SET(&writeEvent, event.ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0,
             NULL);
      kevent(mKq, &writeEvent, 1, NULL, 0, NULL);
    }

  } else {
    std::cout << "Connection closed: " << event.ident << std::endl;
    if (mClients.find(event.ident) != mClients.end()) {
      removeKqueueReadEvents(event.ident);
      removeKqueueWriteEvents(event.ident);
      Client::deleteClient(event.ident);
    }
  }
}

void Server::handleWriteEvent(struct kevent& event) {
  removeKqueueWriteEvents(event.ident);
  std::string::iterator itPos;
  while ((itPos = std::find(mBuffers[event.ident].begin(),
                            mBuffers[event.ident].end(), '\n')) !=
         mBuffers[event.ident].end()) {
    Message parsedMessage = MessageHandler::parseMessage(
        mBuffers[event.ident].substr(0, itPos - mBuffers[event.ident].begin()));

    mBuffers[event.ident].erase(mBuffers[event.ident].begin(),
                                itPos + 1);  // CRLF 제거

    std::cout << parsedMessage << std::endl;
    std::string replyStr = CommandHandler::getInstance()->handleCommand(
        *mClients[event.ident], parsedMessage);
    if (replyStr == "") continue;

    std::cout << "Reply: " << replyStr << std::endl;
    send(event.ident, replyStr.c_str(), replyStr.size(), 0);
  }
}

void Server::run() {
  int nev;

  while (Server::signal == false) {
    try {
      if ((nev = kevent(mKq, NULL, 0, mEvents, MAX_EVENTS, NULL)) == -1 ||
          Server::signal)
        throw std::runtime_error(std::strerror(errno));

      for (int i = 0; i < nev; i++) {
        if (mEvents[i].flags & EV_EOF) {
          Client::deleteClient(mEvents[i].ident);
          continue;
        }
        if (static_cast<int>(mEvents[i].ident) == mListenFd) {
          try {
            /* code */
            handleListenEvent();
          } catch (const std::exception& e) {
            std::cerr << "Listen " << e.what() << '\n';
          }

        } else if (mEvents[i].filter == EVFILT_READ)
          handleReadEvent(mEvents[i]);
        else if (mEvents[i].filter == EVFILT_WRITE)
          handleWriteEvent(mEvents[i]);
      }
    } catch (std::runtime_error& e) {
      std::cerr << e.what() << std::endl;
      errno = 0;
    }
  }
  // 종료 시그널이 발생하면 서버 종료 처리
}

Server::Server(int argc, char* argv[]) {
  errno = 0;

  if (argc != 3)
    throw std::runtime_error("Usage: " + std::string(argv[0]) +
                             " <port> <password>");

  initServerInfo(argv);
  init();
}

Server* Server::getInstance(int argc, char* argv[]) {
  static Server instance(argc, argv);

  return &instance;
}

Server::~Server() {
  for (std::map<int, Client*>::iterator it = mClients.begin();
       it != mClients.end(); ++it) {
    close(it->first);
    delete it->second;
  }

  for (std::map<std::string, Channel*>::iterator it = mChannels.begin();
       it != mChannels.end(); ++it)
    delete it->second;

  close(mListenFd);
  close(mKq);
}

void Server::removeChannel(const std::string& channelName) {
  std::map<std::string, Channel*>::iterator it = mChannels.find(channelName);
  if (it != mChannels.end()) {
    delete it->second;    // 동적 할당된 채널 삭제
    mChannels.erase(it);  // 채널 목록에서 제거
  }
}

void Server::removeClientChannel(Client& client) {
  for (std::map<std::string, Channel*>::iterator it = mChannels.begin();
       it != mChannels.end();) {
    Channel* channel = it->second;
    channel->removeUser(client);
    client.removeChannel(channel);
    if (channel->isEmpty()) {
      delete channel;
      it = mChannels.erase(it);
    } else
      ++it;
  }
}

/**
 * signal
 */
bool Server::signal = false;
void Server::signalHandler(int signum) {
  (void)signum;
  Server::signal = true;
}
void Server::removeKqueueWriteEvents(int fd) {
  // 쓰기 이벤트 제거
  struct kevent writeEvent;
  EV_SET(&writeEvent, fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
  kevent(mKq, &writeEvent, 1, NULL, 0, NULL);
}

void Server::removeKqueueReadEvents(int sockFd) {
  struct kevent readEvent;
  EV_SET(&readEvent, sockFd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
  kevent(mKq, &readEvent, 1, NULL, 0, NULL);
}