#pragma once

#include <algorithm>
#include <ctime>
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
  std::string mTimeStamp;
  size_t mMaxUser;
  std::vector<Client *> mUserlist;
  std::vector<Client *> mInvitedList;
  std::vector<Client *> mGMList;

 public:
  Channel();
  Channel(const std::string &channelName);
  ~Channel();

  void setTopic(const std::string &topic);
  void setChannelKey(const std::string &key);
  void setMaxUser(int maxUser) { mMaxUser = maxUser; }
  void setUserListAdd(Client &client) { mUserlist.push_back(&client); }
  void setUserListSub(Client &client) {
    mUserlist.erase(std::remove(mUserlist.begin(), mUserlist.end(), &client),
                    mUserlist.end());
  }
  void setChannelModeAdd(const char c);
  void setChannelModeSub(const char c);
  void setInvitedListAdd(Client &client) { mInvitedList.push_back(&client); }
  void setInvitedListSub(Client &client) {
    mInvitedList.erase(
        std::remove(mInvitedList.begin(), mInvitedList.end(), &client),
        mInvitedList.end());
  }
  void setGMListAdd(Client &client) { mGMList.push_back(&client); }
  void setGMListSub(Client &client) {
    mGMList.erase(std::remove(mGMList.begin(), mGMList.end(), &client),
                  mGMList.end());
  }

  const std::string &getChannelName() const { return mChannelName; }
  const std::string &getTopic() const { return mTopic; }
  size_t getMaxUser() const { return mMaxUser; }
  const std::string &getChannelMode() const { return mChannelMode; }
  const std::vector<Client *> &getUserList() { return mUserlist; }
  const std::vector<Client *> &getInvitedList() { return mInvitedList; }
  const std::vector<Client *> &getGMList() { return mGMList; }
  const std::string &getChannelKey() const { return mChannelKey; }
  const std::string &getTimeStamp() const { return mTimeStamp; }

  static Channel *findChannel(const std::string &channelName);
  static Channel *createChannel(Client &client, const std::string &channelName);
  static void deleteChannel(const std::string &channelName);

  bool isUserInChannel(std::string nickName);
  void sendPrivmsg(Client &client, const std::string &message);
  void sendPart(std::string message);
  void removeUser(Client &client);
  bool isEmpty();
  bool hasChannel(Client &client);
  bool isOperator(Client &client);
};
