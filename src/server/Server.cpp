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

  EV_SET(&mChangeEvent, mListenFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
  if (kevent(mKq, &mChangeEvent, 1, NULL, 0, NULL) == -1)
    throw std::runtime_error(std::strerror(errno));
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
  mServerConf.serverName = "irc.local";
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

  EV_SET(&mChangeEvent, connFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
  if (kevent(mKq, &mChangeEvent, 1, NULL, 0, NULL) == -1)
    throw std::runtime_error(std::strerror(errno));
  Client::createClient(connFd, inet_ntoa(clientAddr.getAddr()->sin_addr));

  if (errno) throw std::runtime_error(std::strerror(errno));
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
      EV_SET(&mChangeEvent, event.ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0,
             NULL);
      if (kevent(mKq, &mChangeEvent, 1, NULL, 0, NULL) == -1) {
        std::cerr << std::strerror(errno) << std::endl;
        errno = 0;
      }
    }

  } else {
    std::cout << "Connection closed: " << event.ident << std::endl;
    struct kevent deleteEvent;
    EV_SET(&deleteEvent, event.ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    if (kevent(mKq, &deleteEvent, 1, NULL, 0, NULL) == -1)
      throw std::runtime_error(std::strerror(errno));
    struct kevent deleteWriteEvent;
    EV_SET(&deleteWriteEvent, event.ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    if (kevent(mKq, &deleteWriteEvent, 1, NULL, 0, NULL) == -1) {
      perror("kevent - EV_DELETE WRITE");
    }

    if (mClients.find(event.ident) != mClients.end()) {
      Client* client = mClients[event.ident];
      for (std::map<std::string, Channel*>::const_iterator it =
               mChannels.cbegin();
           it != mChannels.cend();) {
        it->second->removeUser(*client);
        client->removeChannel(it->second);
        if (it->second->isEmpty()) {
          delete it->second;
          it = mChannels.erase(it);
        } else
          ++it;
      }
      Client::deleteClient(event.ident);
    }
    close(event.ident);
    errno = 0;
  }
  // if (errno) {
  //   std::cerr << std::strerror(errno) << std::endl;
  //   errno = 0;
  //   close(event.ident);
  // }
}

void Server::handleWriteEvent(struct kevent& event) {
  EV_SET(&mChangeEvent, event.ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
  if (kevent(mKq, &mChangeEvent, 1, NULL, 0, NULL) == -1) {
    std::cout << mKq << std::endl;
    throw std::runtime_error(std::strerror(errno));
  }
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
    if (replyStr.find("ERROR :Closing link:") != std::string::npos) {
      close(event.ident);
      Client::deleteClient(event.ident);
    }
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
        if (mEvents[i].flags & EV_ERROR) {
          if (static_cast<int>(mEvents[i].ident) == mListenFd) {
            fprintf(stderr, "Error on listening socket: %s\n",
                    strerror(mEvents[i].data));
            continue;
          } else {
            fprintf(stderr, "Error on socket %llu: %s\n",
                    (unsigned long long)mEvents[i].ident,
                    strerror(mEvents[i].data));
            close(mEvents[i].ident);
            continue;
          }
        }
        if (static_cast<int>(mEvents[i].ident) == mListenFd)
          handleListenEvent();
        else if (mEvents[i].filter == EVFILT_READ)
          handleReadEvent(mEvents[i]);
        else if (mEvents[i].filter == EVFILT_WRITE)
          handleWriteEvent(mEvents[i]);
      }
    } catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      std::cerr << "Thrown from: " << __FILE__ << ":" << __LINE__ << std::endl;
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

/**
 * signal
 */
bool Server::signal = false;
void Server::signalHandler(int signum) {
  (void)signum;
  Server::signal = true;
}
