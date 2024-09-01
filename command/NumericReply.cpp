#include "NumericReply.hpp"

NumericReply::NumericReply() : mReplies(500) {
  mReplies[RPL_WELCOME] = ":Welcome to the Localnet IRC Network";
  mReplies[ERR_NONICKNAMEGIVEN] = ":No nickname given";
  mReplies[ERR_ERRONEUSNICKNAME] = ":Erroneus nickname";
  mReplies[ERR_NICKNAMEINUSE] = ":Nickname is already in use";
  mReplies[ERR_NICKCOLLISION] = ":Nickname collision KILL";
  mReplies[ERR_NEEDMOREPARAMS] = ":Not enough parameters";
  mReplies[ERR_ALREADYREGISTRED] = ":You may not reregister";
}

const std::string& NumericReply::getReply(int code) { return mReplies[code]; }

NumericReply::~NumericReply() {}
