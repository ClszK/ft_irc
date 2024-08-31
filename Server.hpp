#pragma once

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

class Server {
 public:
 private:
  int mListenFd, mKq;
  struct sockaddr_in mServerAddr;
  struct kevent mChangeEvent, mEvents[10];
  char buffer[1024];

  Server();
  ~Server();
};