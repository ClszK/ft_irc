#include "client/Client.hpp"

#include "channel/Channel.hpp"
#include "server/Server.hpp"

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

const std::vector<Channel*>& Client::getConnectedChannels() const {
  return mConnectedChannels;
}

int Client::getPort() const {
  return Server::getInstance()->getServerConf().port;
}

bool Client::isPasswordValid() const {
  return mPassword == Server::getInstance()->getServerConf().password;
}

void Client::createClient(const int sockFd, char* clientIp) {
  Server::getInstance()->setClient(sockFd, new Client(sockFd, clientIp));
}

void Client::deleteClient(const int sockFd) {
  Client* client = findClient(sockFd);
  Server* server = Server::getInstance();
  std::map<std::string, Channel*> channels = server->getChannels();
  if (client == NULL) return;

  if (channels.size()) {
    for (std::map<std::string, Channel*>::iterator it = channels.begin();
         it != channels.end();) {
      Channel* channel = it->second;
      channel->removeUser(*client);
      client->removeChannel(channel);
      if (channel->isEmpty()) {
        delete channel;
        it = channels.erase(it);
      } else
        ++it;
    }
  }
  server->setClient(sockFd, NULL);
  server->removeBuffer(sockFd);
  std::cout << "Connection closed: " << sockFd << std::endl;
  server->removeKqueueReadEvents(sockFd);
  close(sockFd);
}

Client* Client::findClient(const std::string& nickName) {
  std::map<int, Client*> clients = Server::getInstance()->getClients();
  for (std::map<int, Client*>::iterator it = clients.begin();
       it != clients.end(); ++it) {
    if (it->second->getNickName() == nickName) return it->second;
  }
  return NULL;
}

Client* Client::findClient(const int sockFd) {
  std::map<int, Client*> clients = Server::getInstance()->getClients();
  if (clients.find(sockFd) != clients.end()) return clients[sockFd];
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

void Client::removeChannel(Channel* channel) {
  mConnectedChannels.erase(std::remove(mConnectedChannels.begin(),
                                       mConnectedChannels.end(), channel),
                           mConnectedChannels.end());
}