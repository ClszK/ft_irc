#pragma once
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <csignal>
#include <ctime>
#include <iostream>
#include <map>
#include <sstream>

#include "server/IRCConstants.hpp"
#include "utils/SocketAddr.hpp"

class Client;
class Channel;

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
  static Server* getInstance(int argc = 0, char* argv[] = NULL);
  void run();

  const ServerConf& getServerConf() const { return mServerConf; }
  const std::map<int, Client*>& getClients() const { return mClients; }
  const std::map<std::string, Channel*>& getChannels() const {
    return mChannels;
  }

  void setChannel(const std::string& channelName, Channel* channel);
  void setClient(const int sockFd, Client* client);

  void removeChannel(const std::string& channelName);

  void removeKqueueWriteEvents(int sockFd);
  void removeKqueueReadEvents(int sockFd);
  void removeBuffer(int sockFd) { mBuffers.erase(sockFd); }
  void removeClientChannel(Client& client);

  static void signalHandler(int signum);

 private:
  int mListenFd, mKq;
  SocketAddr mServerAddr;
  struct kevent mEvents[MAX_EVENTS];
  ServerConf mServerConf;
  std::map<int, std::string> mBuffers;
  std::map<int, Client*> mClients;
  std::map<std::string, Channel*> mChannels;
  static bool signal;

  void init();
  void initServerInfo(char* argv[]);
  void handleListenEvent();
  void handleReadEvent(struct kevent& event);
  void handleWriteEvent(struct kevent& event);

  Server(int argc, char* argv[]);
  ~Server();
};