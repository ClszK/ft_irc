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
  bool mTopicLocked;
  int mMaxUser;
  std::set<std::string> mClientlist;
  std::set<std::string> mInvitedList;
  std::set<std::string> mGMList;

 public:
  Channel();
  ~Channel();

  int setChannelName(const std::string& channelName);
  int setTopic(const std::string& topic);
  int setTopicLocked(const std::string& nickName);
  void setMaxUser(int maxUser) { mMaxUser = maxUser; }
  void setClientList(const std::string& nickName) {
    mClientlist.insert(nickName);
  }
  int setChannelMode(Client& client, const std::string& channelName,
                     const std::string& mode);
  int setChannelKey(Client& client, const std::string& channelName,
                    const std::string& key);
  int setChannelUserLimit(const std::string& channelName, int userLimit);
  int setChannelTopic(Client& client, const std::string& channelName,
                      const std::string& topic);
  void setInvitedList(const std::string& nickName) {
    mInvitedList.insert(nickName);
  }
  void setGMList(const std::string& nickName) { mGMList.insert(nickName); }

  const std::string& getChannelName() const { return mChannelName; }
  const std::string& getTopic() const { return mTopic; }
  int getMaxUser() const { return mMaxUser; }

  bool isOperator(Client& client) const;

  static bool findChannel(const std::string& channelName);
  static void createChannel(Client& client, const std::string& channelName);
  static void deleteChannel(Client& client, const std::string& channelName);
};
