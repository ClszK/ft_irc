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
  return Server::getInstance()->getServerConf().serverName;
}

const std::string& Client::getHostName() const {
  return Server::getInstance()->getServerConf().hostName;
}

const std::string& Client::getVersion() const {
  return Server::getInstance()->getServerConf().version;
}

const std::string& Client::getCreatedTime() const {
  return Server::getInstance()->getServerConf().createdTime;
}

const std::string& Client::getAvailableUserMode() const {
  return Server::getInstance()->getServerConf().availableUserMode;
}

const std::string& Client::getAvailableChannelMode() const {
  return Server::getInstance()->getServerConf().availableChannelMode;
}

int Client::getPort() const {
  return Server::getInstance()->getServerConf().port;
}

bool Client::isPasswordValid() const {
  return mPassword == Server::getInstance()->getServerConf().password;
}

void Client::createClient(const int sockFd) {
  Server::getInstance()->setClient(sockFd, new Client(sockFd));
}

void Client::deleteClient(const int sockFd) {
  Server::getInstance()->setClient(sockFd, NULL);
}

Client* Client::findClient(const std::string& nickName) {
  std::map<int, Client*> clients = Server::getInstance()->getClients();
  for (std::map<int, Client*>::iterator it = clients.begin();
       it != clients.end(); ++it) {
    if (it->second->getNickName() == nickName) return it->second;
  }
  return NULL;
}

void Client::sendPrivmsg(const std::string& message) {
  if (mSockFd == -1) return;
  send(mSockFd, message.c_str(), message.size(), 0);
}

void Client::sendPart(const std::string& message) {
  if (mSockFd == -1) return;
  send(mSockFd, message.c_str(), message.size(), 0);
}

void Client::removeChannel(const std::string& channelName) {
  mConnectedChannels.erase(channelName);
}