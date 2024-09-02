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
#include "Channel.hpp"
#include "SocketAddr.hpp"

#define BUF_SIZE 512
#define MAX_EVENTS 1024

class Server {
 public:
  static Server* getInstance(int argc, char* argv[]);
  void run();

  const std::string& getPassword() const { return mPassword; }

 private:
  int mPort, mListenFd, mKq;
  SocketAddr mServerAddr;
  struct kevent mChangeEvent, mEvents[MAX_EVENTS];
  std::string mPassword;
  std::string mServerName;
  std::map<int, Client> mClients;
  std::map<int, std::string> mBuffers;
  std::map<std::string, Channel*> mChannels;
  CommandHandler mCommandHandler;

  void init();
  void handleListenEvent();
  void handleReadEvent(struct kevent& event);
  void handleWriteEvent(struct kevent& event);
  void sendReplyToClient(struct kevent& event, ReplyPair reply);

  Server(int argc, char* argv[]);
  ~Server();
};