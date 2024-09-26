#include "channel/Channel.hpp"

#include "client/Client.hpp"

void Channel::setChannelKey(const std::string &key) { mChannelKey = key; }

void Channel::setTopic(const std::string &topic) {
  if (topic.size() > TOPICLEN)
    mTopic = topic.substr(0, TOPICLEN);
  else
    mTopic = topic;
}

void Channel::setChannelModeAdd(const char c) {
  if (mChannelMode.find(c) == std::string::npos) mChannelMode += c;
}

void Channel::setChannelModeSub(const char c) {
  mChannelMode.erase(std::remove(mChannelMode.begin(), mChannelMode.end(), c),
                     mChannelMode.end());
}

Channel *Channel::findChannel(const std::string &channelName) {
  const std::map<std::string, Channel *> &channelList =
      Server::getInstance()->getChannels();

  std::map<std::string, Channel *>::const_iterator it;
  it = channelList.find(channelName);
  if (it == channelList.end()) return NULL;
  return it->second;
}

Channel *Channel::createChannel(Client &client,
                                const std::string &channelName) {
  Channel *channel = new Channel(channelName);

  channel->setUserListAdd(client);
  channel->setGMListAdd(client);

  Server::getInstance()->setChannel(channelName, channel);
  return channel;
}

void Channel::deleteChannel(const std::string &channelName) {
  if (!findChannel(channelName)) return;

  Server::getInstance()->setChannel(channelName, NULL);
}

bool Channel::isUserInChannel(std::string nickName) {
  for (size_t i = 0; i < mUserlist.size(); i++) {
    if (mUserlist[i]->getNickName() == nickName) return true;
  }
  return false;
}

void Channel::sendPrivmsg(Client &client, const std::string &message) {
  for (size_t i = 0; i < mUserlist.size(); i++) {
    if (mUserlist[i] != &client) mUserlist[i]->sendPrivmsg(message);
  }
}

void Channel::sendPart(std::string message) {
  for (size_t i = 0; i < mUserlist.size(); i++) {
    mUserlist[i]->sendPart(message);
  }
}

void Channel::removeUser(Client &client) {
  std::vector<Client *>::iterator it =
      std::find(mUserlist.begin(), mUserlist.end(), &client);
  if (it != mUserlist.end()) {
    mUserlist.erase(it);
    if (mGMList.size() > 0) {
      it = std::find(mGMList.begin(), mGMList.end(), &client);
      if (it != mGMList.end()) mGMList.erase(it);
    }
    if (mInvitedList.size() > 0) {
      it = std::find(mInvitedList.begin(), mInvitedList.end(), &client);
      if (it != mInvitedList.end()) mInvitedList.erase(it);
    }
  }
}

bool Channel::isEmpty() { return mUserlist.empty(); }

bool Channel::hasChannel(Client &client) {
  return std::find(mUserlist.begin(), mUserlist.end(), &client) !=
         mUserlist.end();
}

Channel::Channel() : mTopic(""), mChannelMode("tn"), mMaxUser(5000) {}

Channel::Channel(const std::string &channelName)
    : mChannelName(channelName),
      mTopic(""),
      mChannelMode("tn"),
      mMaxUser(5000) {
  mTimeStamp = std::time(NULL);
  std::time_t now = std::time(NULL);
  std::stringstream ss;
  ss << now;
  mTimeStamp = ss.str();
}

Channel::~Channel() {}

bool Channel::isOperator(Client &client) {
  return std::find(mGMList.begin(), mGMList.end(), &client) != mGMList.end();
}