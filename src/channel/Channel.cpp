#include "channel/Channel.hpp"

int Channel::setChannelName(const std::string& channelName) {
  if (!StringUtility::isValidChannelName(channelName))
    return ERR_INVALIDCHANNELNAME;
  mChannelName = channelName;
  return 0;
}

int Channel::setTopic(const std::string& topic) {
  if (mTopicLocked) return ERR_CHANOPRIVSNEEDED;

  if (topic.size() > TOPICLEN) return ERR_TOPICTOOLONG;
  mTopic = topic;
  return 0;
}

int Channel::setTopicLocked(Client& client) {
  if (mChannelMode.find('t') == std::string::npos) return ERR_CHANOPRIVSNEEDED;
  if (isOperator(client)) return ERR_CHANOPRIVSNEEDED;
  mTopicLocked = !mTopicLocked;
  return 0;
}

bool Channel::isOperator(Client& client) const {
  return mGMList.find(&client) != mGMList.end();
}

bool Channel::findChannel(const std::string& channelName) {
  const std::map<std::string, Channel*>& channelList =
      Server::getInstance()->getChannels();

  if (channelList.find(channelName) == channelList.end()) return false;
  return true;
}

void Channel::createChannel(Client& client, const std::string& channelName) {
  if (findChannel(channelName)) return;

  Channel* channel = new Channel();

  channel->setChannelName(channelName);
  channel->setClientList(client);
  channel->setGMList(client);

  Server::getInstance()->setChannel(channelName, channel);
}

void Channel::deleteChannel(Client& client, const std::string& channelName) {
  if (!findChannel(channelName)) return;

  Server::getInstance()->setChannel(channelName, NULL);
}

Channel::Channel()
    : mTopic(""), mChannelMode("tn"), mTopicLocked(true), mMaxUser(5000) {}

Channel::~Channel() {}