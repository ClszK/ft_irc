#pragma once

#include <string>
#include <vector>

class Client {
 private:
  int mSockFd;
  std::string mNickName;
  std::string mUserName;
  std::string mRealName;
  std::string mUserMode;
  bool mIsCertified;

 public:
  Client(const int sockFd);
  ~Client();
};