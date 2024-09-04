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
#include <ctime>
#include <iostream>
#include <map>
#include <sstream>

#include "channel/Channel.hpp"
#include "command/CommandHandler.hpp"
#include "server/IRCConstants.hpp"
#include "utils/SocketAddr.hpp"

struct ServerConf {
  std::string serverName;
  std::string hostName;
  std::string version;
  std::string password;
  std::string createdTime;
  std::string availableUserMode;
  std::string availableChannelMode;
  int port;
};

class Server {
 public:
  static Server* getInstance(int argc, char* argv[]);
  void run();

  const ServerConf& getServerConf() const { return mServerConf; }

 private:
  int mListenFd, mKq;
  SocketAddr mServerAddr;
  struct kevent mChangeEvent, mEvents[MAX_EVENTS];
  ServerConf mServerConf;
  std::map<int, Client> mClients;
  std::map<int, std::string> mBuffers;
  std::map<std::string, Channel> mChannels;
  CommandHandler mCommandHandler;

  void init();
  void initServerInfo(char* argv[]);
  void handleListenEvent();
  void handleReadEvent(struct kevent& event);
  void handleWriteEvent(struct kevent& event);

  Server(int argc, char* argv[]);
  ~Server();
};