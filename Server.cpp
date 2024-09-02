#include "Server.hpp"

void Server::init() {
  NumericReply::initializeReplies();
  mServerName = "IRCServer";
  if ((mListenFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    throw std::runtime_error(std::strerror(errno));

  mServerAddr = SocketAddr(mPort);

  if (bind(mListenFd, mServerAddr, sizeof(struct sockaddr)) == -1 ||
      listen(mListenFd, MAX_EVENTS) == -1 || (mKq = kqueue()) == -1)
    throw std::runtime_error(std::strerror(errno));

  EV_SET(&mChangeEvent, mListenFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
  if (kevent(mKq, &mChangeEvent, 1, NULL, 0, NULL) == -1)
    throw std::runtime_error(std::strerror(errno));
}

void Server::handleListenEvent() {
  int connFd;
  if ((connFd = accept(mListenFd, NULL, NULL)) == -1 ||
      fcntl(connFd, F_SETFL, O_NONBLOCK) == -1)
    throw std::runtime_error(std::strerror(errno));

  EV_SET(&mChangeEvent, connFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
  if (kevent(mKq, &mChangeEvent, 1, NULL, 0, NULL) == -1)
    throw std::runtime_error(std::strerror(errno));
  mClients.insert(std::make_pair(connFd, Client(connFd, &mPassword, this)));

  if (errno) throw std::runtime_error(std::strerror(errno));
}

void Server::handleReadEvent(struct kevent& event) {
  char buffer[BUF_SIZE];

  int n = recv(event.ident, buffer, sizeof(buffer) - 1, 0);
  buffer[n] = '\0';
  if (n > 0) {
    mBuffers[event.ident] += buffer;

    std::cout << "Recv: " << mBuffers[event.ident] << std::endl;
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
    mClients.erase(event.ident);
    close(event.ident);
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

  std::string::iterator itPos = std::find(mBuffers[event.ident].begin(),
                                          mBuffers[event.ident].end(), '\n');

  Message parsedMessage = MessageHandler::parseMessage(
      mBuffers[event.ident].substr(0, itPos - mBuffers[event.ident].begin()));

  mBuffers[event.ident].erase(mBuffers[event.ident].begin(),
                              itPos + 1);  // CRLF 제거
  std::cout << parsedMessage << std::endl;

  ReplyPair reply =
      mCommandHandler.handleCommand(mClients[event.ident], parsedMessage);
  sendReplyToClient(event, reply);
}

void Server::sendReplyToClient(struct kevent& event, ReplyPair reply) {
  std::cout << "Reply: " << reply.first << " " << reply.second << std::endl;
  std::stringstream ss;
  std::string message;
  if (reply.first == 0 && reply.second.empty())
    return;
  else if (reply.first < 0) {
    ss << reply.second << "\n";
    message = ss.str();
    send(event.ident, message.c_str(), message.size(), 0);
    mClients.erase(event.ident);
    close(event.ident);
    return;
  }

  ss << ":" << mServerName;
  if (reply.first > 0) ss << " " << reply.first;
  ss << " " << reply.first << " " << reply.second << "\n";

  message = ss.str();
  std::cout << "Send: " << message;
  send(event.ident, message.c_str(), message.size(), 0);
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

Server::Server(int argc, char* argv[]) : mPassword(argv[2]) {
  char* end;

  errno = 0;

  if (argc != 3)
    throw std::runtime_error("Usage: " + std::string(argv[0]) +
                             " <port> <password>");
  long port = std::strtol(argv[1], &end, 10);
  if (errno == ERANGE || *end != '\0' || port < 0 || port > 65535)
    throw std::runtime_error("Invalid port number");

  mPort = port;

  init();
}

Server* Server::getInstance(int argc, char* argv[]) {
  static Server instance(argc, argv);

  return &instance;
}

Server::~Server() {}
