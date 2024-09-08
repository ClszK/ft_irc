#pragma once

#include <algorithm>
#include <map>
#include <string>

#include "server/IRCConstants.hpp"
#include "server/Server.hpp"
#include "utils/NumericReply.hpp"
#include "utils/StringUtility.hpp"

class Client;

class Channel {
 private:
  std::string mChannelName;
  std::string mTopic;
  std::string mChannelMode;
  std::string mChannelKey;
  bool mTopicLocked;
  int mMaxUser;
  std::vector<Client*> mUserlist;
  std::vector<Client*> mInvitedList;
  std::vector<Client*> mGMList;

 public:
  Channel();
  Channel(const std::string& channelName);
  ~Channel();

  int setTopic(const std::string& topic);
  int setTopicLocked(Client& client);
  int setChannelKey(Client& client, const std::string& key);
  void setMaxUser(int maxUser) { mMaxUser = maxUser; }
  void setUserList(Client& client) { mUserlist.push_back(&client); }
  int setChannelMode(Client& client, const std::string& mode);
  int setChannelUserLimit(Client& client, int userLimit);
  int setChannelTopic(Client& client, const std::string& topic);
  void setInvitedList(Client& client) { mInvitedList.push_back(&client); }
  void setGMList(Client& client) { mGMList.push_back(&client); }

  const std::string& getChannelName() const { return mChannelName; }
  const std::string& getTopic() const { return mTopic; }
  int getMaxUser() const { return mMaxUser; }
  std::vector<Client*>& getUserList() { return mUserlist; }
  std::vector<Client*>& getInvitedList() { return mInvitedList; }
  std::vector<Client*>& getGMList() { return mGMList; }

  static Channel* findChannel(const std::string& channelName);
  static Channel* createChannel(Client& client, const std::string& channelName);
  static void deleteChannel(const std::string& channelName);
};
