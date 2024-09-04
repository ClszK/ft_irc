#pragma once

#include <map>
#include <set>
#include <string>

#include "server/IRCConstants.hpp"
#include "utils/NumericReply.hpp"
#include "utils/StringUtility.hpp"

class Client;

class Channel {
 private:
  std::string mChannelName;
  std::string mTopic;
  std::string mChannelMode;
  bool mTopicLocked;
  int mMaxUser;
  std::map<std::string, Client*> mClientlist;
  std::set<Client*> mInvitedList;
  std::set<Client*> mGMList;

 public:
  Channel();
  ~Channel();

  int setChannelName(const std::string& channelName);
  int setTopic(const std::string& topic);
  int setTopicLocked(bool topicLocked);
  void setMaxUser(int maxUser) { mMaxUser = maxUser; }

  const std::string& getChannelName() const { return mChannelName; }
  const std::string& getTopic() const { return mTopic; }
  bool isTopicLocked() const { return mTopicLocked; }
  int getMaxUser() const { return mMaxUser; }

  static int createChannel(Client& client, const std::string& channelName);
};