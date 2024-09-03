#include "client/Client.hpp"

#include "server/Server.hpp"

bool Client::setNickName(const std::string& nickName) {
  if (!StringUtility::isValidNickName(nickName)) return false;
  return mNickName = nickName, true;
}

bool Client::setUserName(const std::string& userName) {
  if (!StringUtility::isValidUserName(userName)) return false;
  return mUserName = userName, true;
}

const std::string& Client::getServerName() const {
  return mServer->getServerName();
}

const std::string& Client::getHostName() const {
  return mServer->getHostName();
}

bool Client::isPasswordValid() const {
  return mPassword == mServer->getPassword();
}

Client::Client() : mServer(NULL) {}

Client::Client(const int sockFd, Server* server)
    : mSockFd(sockFd), mServer(server) {}

Client::~Client() {}
