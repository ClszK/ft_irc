#pragma once

#include <string>
#include <vector>

#include "utils/StringUtility.hpp"

class Channel;

class Client {
 private:
  int mSockFd;
  bool mIsRegistered;
  std::string mNickName;
  std::string mUserName;
  std::string mRealName;
  std::string mUserMode;
  std::string mPassword;
  std::string mClientIp;
  std::vector<Channel *> mConnectedChannels;

 public:
  void setRegistered(bool isRegistered) { mIsRegistered = isRegistered; }
  void setNickName(const std::string &nickName) { mNickName = nickName; }
  void setUserName(const std::string &userName) { mUserName = userName; }
  void setRealName(const std::string &realName) { mRealName = realName; }
  void setUserMode(const std::string &userMode) { mUserMode = userMode; }
  void setPassword(const std::string &password) { mPassword = password; }
  void setConnectedChannel(Channel *channel) {
    mConnectedChannels.push_back(channel);
  }
  int getSockFd() const { return mSockFd; }
  int getRegistered() const { return mIsRegistered; }
  const std::string &getNickName() const { return mNickName; }
  const std::string &getUserName() const { return mUserName; }
  const std::string &getRealName() const { return mRealName; }
  const std::string &getUserMode() const { return mUserMode; }
  const std::string &getPassword() const { return mPassword; }
  const std::string &getClientIp() const { return mClientIp; }
  const std::string &getServerName() const;
  const std::string &getHostName() const;
  const std::string &getVersion() const;
  const std::string &getCreatedTime() const;
  const std::string &getAvailableUserMode() const;
  const std::string &getAvailableChannelMode() const;
  const std::vector<Channel *> &getConnectedChannels() const;
  int getPort() const;

  bool isPasswordValid() const;

  static void createClient(const int sockFd, char *clientIp);
  static void deleteClient(const int sockFd);
  static Client *findClient(const std::string &nickName);
  static Client *findClient(const int sockFd);

  void sendPrivmsg(const std::string &message);
  void sendPart(const std::string &message);
  void removeChannel(Channel *channel);

  Client() {};
  Client(const int sockFd, char *clientIp)
      : mSockFd(sockFd), mIsRegistered(false) {
    if (strcmp(clientIp, "0.0.0.0") == 0) {
      mClientIp = "127.0.0.1";
    } else
      mClientIp = clientIp;
  };
  ~Client() {};
};
