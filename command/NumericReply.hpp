#pragma once

#include <string>
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

class NumericReply {
 private:
  std::vector<std::string> mReplies;

 public:
  NumericReply();
  const std::string& getReply(int code);
  ~NumericReply();
};