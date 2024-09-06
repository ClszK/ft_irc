#pragma once

#include <map>
#include <set>
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
  std::set<Client*> mUserlist;
  std::set<Client*> mInvitedList;
  std::set<Client*> mGMList;

 public:
  Channel();
  Channel(const std::string& channelName);
  ~Channel();

  int setTopic(const std::string& topic);
  int setTopicLocked(Client& client);
  int setChannelKey(Client& client, const std::string& key);
  void setMaxUser(int maxUser) { mMaxUser = maxUser; }
  void setUserList(Client& client) { mUserlist.insert(&client); }
  int setChannelMode(Client& client, const std::string& mode);
  int setChannelUserLimit(Client& client, int userLimit);
  int setChannelTopic(Client& client, const std::string& topic);
  void setInvitedList(Client& client) { mInvitedList.insert(&client); }
  void setGMList(Client& client) { mGMList.insert(&client); }

  const std::string& getChannelName() const { return mChannelName; }
  const std::string& getTopic() const { return mTopic; }
  int getMaxUser() const { return mMaxUser; }

  static bool findChannel(const std::string& channelName);
  static void createChannel(Client& client, const std::string& channelName);
  static void deleteChannel(Client& client, const std::string& channelName);
};
