#pragma once

#include <map>
#include <set>
#include <string>

class Client;

class Channel {
 private:
  std::string mChannelName;
  std::string mTopic;
  std::map<std::string, Client*> mClientlist;
  std::set<Client*> mInvitedList;
  std::set<Client*> mGMList;
  int mChannelMode;
  bool mTopicLocked;
  int mMaxUser;

 public:
  Channel();
  ~Channel();

  void createChannel(Client& client, const std::string& channelName);
};