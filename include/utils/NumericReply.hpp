#pragma once

#include <string>
#include <utility>
#include <vector>

enum Reply {
  RPL_WELCOME = 1,
  RPL_YOURHOST = 2,
  RPL_CREATED = 3,
  RPL_MYINFO = 4,
  RPL_SUPPORT = 5,
  RPL_NAMREPLY = 353,
  RPL_ENDOFNAMES = 366,
  ERR_TOPICTOOLONG = 413,
  ERR_NONICKNAMEGIVEN = 431,
  ERR_ERRONEUSNICKNAME = 432,
  ERR_NICKNAMEINUSE = 433,
  ERR_NICKCOLLISION = 436,
  ERR_NOTREGISTERED = 451,
  ERR_NEEDMOREPARAMS = 461,
  ERR_ALREADYREGISTRED = 462,
  ERR_INVALIDUSERNAME = 468,
  ERR_INVALIDCHANNELNAME = 476,
  ERR_CHANOPRIVSNEEDED = 482,
};

typedef std::pair<int, std::string> ReplyPair;

class NumericReply {
 private:
  static std::vector<std::string> mReplies;
  NumericReply();

 public:
  static void initializeReplies();
  static const std::string& getReply(int code);
  ~NumericReply();
};