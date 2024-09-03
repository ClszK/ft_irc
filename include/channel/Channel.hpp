#pragma once

#include <map>
#include <set>
#include <string>

#define MAX_CHANNEL_LENGTH 200

class Client;

class Channel {
 private:
  std::string mChannelName;
  std::string mTopic;
  int mChannelMode;
  bool mTopicLocked;
  int mMaxUser;
  std::map<std::string, Client*> mClientlist;
  std::set<Client*> mInvitedList;
  std::set<Client*> mGMList;

 public:
  Channel();
  ~Channel();

  void setChannelName(const std::string& channelName) {
    mChannelName = channelName;
  }
  void setTopic(const std::string& topic) { mTopic = topic; }
  void setChannelMode(int channelMode) { mChannelMode = channelMode; }
  void setTopicLocked(bool topicLocked) { mTopicLocked = topicLocked; }
  void setMaxUser(int maxUser) { mMaxUser = maxUser; }

  const std::string& getChannelName() const { return mChannelName; }
  const std::string& getTopic() const { return mTopic; }
  int getChannelMode() const { return mChannelMode; }
  bool isTopicLocked() const { return mTopicLocked; }
  int getMaxUser() const { return mMaxUser; }

  void createChannel(Client& client, const std::string& channelName);
};