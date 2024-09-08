#include "channel/Channel.hpp"

int Channel::setChannelKey(Client& client, const std::string& key) {
  if (std::find(mGMList.begin(), mGMList.end(), &client) == mGMList.end())
    return ERR_CHANOPRIVSNEEDED;
  mChannelKey = key;
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
  if (std::find(mGMList.begin(), mGMList.end(), &client) == mGMList.end())
    return ERR_CHANOPRIVSNEEDED;
  mTopicLocked = !mTopicLocked;
  return 0;
}

int Channel::setChannelMode(Client& client, const std::string& mode) {
  if (std::find(mGMList.begin(), mGMList.end(), &client) == mGMList.end())
    return ERR_CHANOPRIVSNEEDED;

  mChannelMode = mode;
  return 0;
}

Channel* Channel::findChannel(const std::string& channelName) {
  const std::map<std::string, Channel*>& channelList =
      Server::getInstance()->getChannels();

  std::map<std::string, Channel*>::const_iterator it;
  it = channelList.find(channelName);
  if (it == channelList.end()) return NULL;
  return it->second;
}

Channel* Channel::createChannel(Client& client,
                                const std::string& channelName) {
  Channel* channel = new Channel(channelName);

  channel->setUserList(client);
  channel->setGMList(client);

  Server::getInstance()->setChannel(channelName, channel);
  return channel;
}

void Channel::deleteChannel(const std::string& channelName) {
  if (!findChannel(channelName)) return;

  Server::getInstance()->setChannel(channelName, NULL);
}

Channel::Channel()
    : mTopic(""), mChannelMode("tn"), mTopicLocked(true), mMaxUser(5000) {}

Channel::Channel(const std::string& channelName)
    : mChannelName(channelName),
      mTopic(""),
      mChannelMode("tn"),
      mTopicLocked(true),
      mMaxUser(5000) {}

Channel::~Channel() {}