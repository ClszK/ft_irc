#include "NumericReply.hpp"

NumericReply::NumericReply() {
  mReplies[RPL_WELCOME] = "RPL_WELCOME";
  mReplies[ERR_NONICKNAMEGIVEN] = ":No nickname given";
  mReplies[ERR_ERRONEUSNICKNAME] = ":Erroneus nickname";
  mReplies[ERR_NICKNAMEINUSE] = ":Nickname is already in use";
  mReplies[ERR_NICKCOLLISION] = ":Nickname collision KILL";
  mReplies[ERR_NEEDMOREPARAMS] = ":Not enough parameters";
  mReplies[ERR_ALREADYREGISTRED] = ":You may not reregister";
}

std::string NumericReply::getReply(int code) const { return mReplies.at(code); }

NumericReply::~NumericReply() {}
