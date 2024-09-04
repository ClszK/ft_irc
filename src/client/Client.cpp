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
  return mServer->getServerConf().serverName;
}

const std::string& Client::getHostName() const {
  return mServer->getServerConf().hostName;
}

const std::string& Client::getVersion() const {
  return mServer->getServerConf().version;
}

const std::string& Client::getCreatedTime() const {
  return mServer->getServerConf().createdTime;
}

const std::string& Client::getAvailableUserMode() const {
  return mServer->getServerConf().availableUserMode;
}

const std::string& Client::getAvailableChannelMode() const {
  return mServer->getServerConf().availableChannelMode;
}

int Client::getPort() const { return mServer->getServerConf().port; }

bool Client::isPasswordValid() const {
  return mPassword == mServer->getServerConf().password;
}

Client::Client() : mServer(NULL) {}

Client::Client(const int sockFd, Server* server)
    : mSockFd(sockFd), mServer(server) {}

Client::~Client() {}
