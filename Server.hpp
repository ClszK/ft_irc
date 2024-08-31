#pragma once

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <iostream>
#include <map>
#include <sstream>

#include "./command/CommandHandler.hpp"
#include "Client.hpp"
#include "MessageHandler.hpp"
#include "SocketAddr.hpp"

#define BUF_SIZE 512
#define MAX_EVENTS 1024

class Server {
 public:
  static Server& getInstance(int argc, const char* argv[]);
  void run();

 private:
  int mPort, mListenFd, mKq;
  SocketAddr mServerAddr;
  struct kevent mChangeEvent, mEvents[10];
  std::string mPassword;
  std::map<int, Client> mClients;
  std::map<int, std::string> mBuffers;

  void init();
  void handleListenEvent();
  void handleReadEvent(struct kevent event);

  Server(int argc, const char* argv[]);
  ~Server();
};