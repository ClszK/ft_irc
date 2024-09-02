#pragma once

#include <string>
#include <utility>
#include <vector>

enum Reply {
  RPL_WELCOME = 1,
  ERR_NONICKNAMEGIVEN = 431,
  ERR_ERRONEUSNICKNAME = 432,
  ERR_NICKNAMEINUSE = 433,
  ERR_NICKCOLLISION = 436,
  ERR_NEEDMOREPARAMS = 461,
  ERR_ALREADYREGISTRED = 462,
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