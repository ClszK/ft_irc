#pragma once

#include <string>

class Server;

class Client {
 private:
  int mSockFd;
  std::string mNickName;
  std::string mUserName;
  std::string mRealName;
  std::string mUserMode;
  std::string mPassword;
  const std::string* mServerPassword;
  Server* mServer;

 public:
  void setNickName(const std::string& nickName) { mNickName = nickName; }
  void setUserName(const std::string& userName) { mUserName = userName; }
  void setRealName(const std::string& realName) { mRealName = realName; }
  void setUserMode(const std::string& userMode) { mUserMode = userMode; }
  void setPassword(const std::string& password) { mPassword = password; }
  int getSockFd() const { return mSockFd; }
  const std::string& getNickName() const { return mNickName; }
  const std::string& getUserName() const { return mUserName; }
  const std::string& getRealName() const { return mRealName; }
  const std::string& getUserMode() const { return mUserMode; }
  const std::string& getPassword() const { return mPassword; }

  bool isPasswordValid(const std::string& password) const {
    return password == *mServerPassword;
  }

  Client();
  Client(const int sockFd, std::string* serverPassword, Server* server);
  ~Client();
};