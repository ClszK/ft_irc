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
  mServerConf.serverName = "IRCServer";
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
  if ((connFd = accept(mListenFd, NULL, NULL)) == -1 ||
      fcntl(connFd, F_SETFL, O_NONBLOCK) == -1)
    throw std::runtime_error(std::strerror(errno));

  EV_SET(&mChangeEvent, connFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
  if (kevent(mKq, &mChangeEvent, 1, NULL, 0, NULL) == -1)
    throw std::runtime_error(std::strerror(errno));
  Client::createClient(connFd);

  if (errno) throw std::runtime_error(std::strerror(errno));
}

void Server::handleReadEvent(struct kevent& event) {
  char buffer[LINELEN];

  int n = recv(event.ident, buffer, sizeof(buffer) - 1, 0);
  buffer[n] = '\0';
  if (n > 0) {
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

  } else if (n == 0) {
    std::cout << "Connection closed: " << event.ident << std::endl;
    if (mClients[event.ident]) close(event.ident);
    Client::deleteClient(event.ident);
  }
  if (errno) {
    std::cerr << std::strerror(errno) << std::endl;
    errno = 0;
    close(event.ident);
  }
}

void Server::handleWriteEvent(struct kevent& event) {
  EV_SET(&mChangeEvent, event.ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
  if (kevent(mKq, &mChangeEvent, 1, NULL, 0, NULL) == -1)
    throw std::runtime_error(std::strerror(errno));
  std::string::iterator itPos;
  while ((itPos = std::find(mBuffers[event.ident].begin(),
                            mBuffers[event.ident].end(), '\n')) !=
         mBuffers[event.ident].end()) {
    Message parsedMessage = MessageHandler::parseMessage(
        mBuffers[event.ident].substr(0, itPos - mBuffers[event.ident].begin()));

    mBuffers[event.ident].erase(mBuffers[event.ident].begin(),
                                itPos + 1);  // CRLF 제거
    std::cout << parsedMessage << std::endl;

    std::string replyStr =
        CommandHandler::getInstance()
            ->handleCommand(*mClients[event.ident], parsedMessage)
            .c_str();
    if (replyStr.empty()) continue;

    std::cout << "Reply: " << replyStr << std::endl;
    send(event.ident, replyStr.c_str(), replyStr.size(), 0);
    if (replyStr == ReplyUtility::makeErrorReply(*mClients[event.ident]))
      Client::deleteClient(event.ident);
  }
}

void Server::run() {
  int nev;

  while (true) {
    if ((nev = kevent(mKq, NULL, 0, mEvents, MAX_EVENTS, NULL)) == -1)
      throw std::runtime_error(std::strerror(errno));

    for (int i = 0; i < nev; i++) {
      if (static_cast<int>(mEvents[i].ident) == mListenFd)
        handleListenEvent();
      else if (mEvents[i].filter == EVFILT_READ)
        handleReadEvent(mEvents[i]);
      else if (mEvents[i].filter == EVFILT_WRITE)
        handleWriteEvent(mEvents[i]);
    }
  }
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
