#pragma once

#include <map>
#include <string>

#include "utils/StringUtility.hpp"

class Channel;

class Client {
 private:
  int mSockFd;
  std::string mNickName;
  std::string mUserName;
  std::string mRealName;
  std::string mUserMode;
  std::string mPassword;
  std::map<std::string, Channel*> mConnectedChannels;

 public:
  bool setNickName(const std::string& nickName);
  bool setUserName(const std::string& userName);
  void setRealName(const std::string& realName) { mRealName = realName; }
  void setUserMode(const std::string& userMode) { mUserMode = userMode; }
  void setPassword(const std::string& password) { mPassword = password; }
  int getSockFd() const { return mSockFd; }
  const std::string& getNickName() const { return mNickName; }
  const std::string& getUserName() const { return mUserName; }
  const std::string& getRealName() const { return mRealName; }
  const std::string& getUserMode() const { return mUserMode; }
  const std::string& getPassword() const { return mPassword; }
  const std::string& getServerName() const;
  const std::string& getHostName() const;
  const std::string& getVersion() const;
  const std::string& getCreatedTime() const;
  const std::string& getAvailableUserMode() const;
  const std::string& getAvailableChannelMode() const;
  int getPort() const;

  bool isPasswordValid() const;

  static void createClient(const int sockFd);
  static void deleteClient(const int sockFd);

  Client() {};
  Client(const int sockFd) : mSockFd(sockFd) {};
  ~Client() {};
};
