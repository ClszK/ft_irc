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
#include "SocketAddr.hpp"

#define BUF_SIZE 512
#define MAX_EVENTS 1024

class CommandHandler;

class Server {
 public:
  static Server& getInstance(int argc, const char* argv[]);
  void run();

  const std::string& getPassword() const { return mPassword; }

 private:
  int mPort, mListenFd, mKq;
  SocketAddr mServerAddr;
  struct kevent mChangeEvent, mEvents[10];
  std::string mPassword;
  std::string mServerName;
  std::map<int, Client> mClients;
  std::map<int, std::string> mBuffers;
  CommandHandler mCommandHandler;

  void init();
  void handleListenEvent();
  void handleReadEvent(struct kevent event);
  void sendReplyToClient(Client& client, std::pair<int, std::string> reply);

  Server(int argc, const char* argv[]);
  ~Server();
};