#include "Server.hpp"

void Server::init() {
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
  while ((connFd = accept(mListenFd, NULL, NULL)) != -1) {
    if (fcntl(connFd, F_SETFL, O_NONBLOCK) == -1)
      throw std::runtime_error(std::strerror(errno));

    EV_SET(&mChangeEvent, connFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    if (kevent(mKq, &mChangeEvent, 1, NULL, 0, NULL) == -1)
      throw std::runtime_error(std::strerror(errno));
    mClients.insert(std::make_pair(connFd, Client(connFd)));
  }

  if (errno != EAGAIN && errno != EWOULDBLOCK)
    throw std::runtime_error(std::strerror(errno));
}

void Server::handleReadEvent(struct kevent event) {
  char buffer[BUF_SIZE];

  int n = recv(event.ident, buffer, sizeof(buffer) - 1, 0);
  if (n > 0) {
    mBuffers[event.ident] += buffer;

    std::string::size_type pos;

    // CRLF("\r\n")을 기준으로 메시지를 구분하여 처리
    while ((pos = mBuffers[event.ident].find("\r\n")) != std::string::npos) {
      std::string message = mBuffers[event.ident].substr(0, pos);
      mBuffers[event.ident].erase(0, pos + 2);  // CRLF 제거

      // 메시지 처리
    }

    EV_SET(&mChangeEvent, event.ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0,
           NULL);
    if (kevent(mKq, &mChangeEvent, 1, NULL, 0, NULL) == -1) {
      std::cerr << std::strerror(errno) << std::endl;
      errno = 0;
    }
  } else if (n == 0) {
    std::cout << "Connection closed: " << event.ident << std::endl;
    close(event.ident);
  } else {
    if (errno != EAGAIN && errno != EWOULDBLOCK) {
      std::cerr << std::strerror(errno) << std::endl;
      errno = 0;
      close(event.ident);
    }
  }
}

void Server::run() {
  int nev;

  while (true) {
    if ((nev = kevent(mKq, NULL, 0, mEvents, MAX_EVENTS, NULL)) == -1)
      throw std::runtime_error(std::strerror(errno));

    for (int i = 0; i < nev; i++) {
      if (mEvents[i].ident == mListenFd)
        handleListenEvent();
      else if (mEvents[i].filter == EVFILT_READ)
        handleReadEvent(mEvents[i]);
    }
  }
}

Server::Server(int argc, const char* argv[]) : mPassword(argv[2]) {
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

Server& Server::getInstance(int argc, const char* argv[]) {
  static Server instance(argc, argv);

  return instance;
}

Server::~Server() {}
