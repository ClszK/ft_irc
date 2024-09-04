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

int Channel::setTopicLocked(bool topicLocked) {
  if (mChannelMode.find('t') == std::string::npos) return ERR_CHANOPRIVSNEEDED;
}

int Channel::createChannel(Client& client, const std::string& channelName) {}

Channel::Channel()
    : mTopic(""), mChannelMode("tn"), mTopicLocked(true), mMaxUser(5000) {}

Channel::~Channel() {}