#include "channel/Channel.hpp"

void Channel::setChannelKey(const std::string& key) { mChannelKey = key; }

int Channel::setTopic(const std::string& topic) {
  if (mTopicLocked) return ERR_CHANOPRIVSNEEDED;

  if (topic.size() > TOPICLEN) return ERR_TOPICTOOLONG;
  mTopic = topic;
  return 0;
}

void Channel::setTopicLocked(bool lock) { mTopicLocked = lock; }

void Channel::setChannelModeAdd(const char c) {
  if (mChannelMode.find(c) == std::string::npos) mChannelMode += c;
}

void Channel::setChannelModeSub(const char c) {
  mChannelMode.erase(std::remove(mChannelMode.begin(), mChannelMode.end(), c),
                     mChannelMode.end());
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

  channel->setUserListAdd(client);
  channel->setGMListAdd(client);

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