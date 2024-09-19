#include "utils/NumericReply.hpp"

std::vector<std::string> NumericReply::mReplies(500);

void NumericReply::initializeReplies() {
  mReplies[RPL_WELCOME] = ":Welcome to the Internet Relay Network";
  mReplies[RPL_YOURHOST] = ":Your host is <servername>, running version <ver>";
  mReplies[RPL_CREATED] = ":This server was created";
  mReplies[RPL_SUPPORT] = ":are supported by this server";
  mReplies[RPL_ENDOFNAMES] = ":End of /NAMES list";
  mReplies[ERR_NONICKNAMEGIVEN] = ":No nickname given";
  mReplies[ERR_ERRONEUSNICKNAME] = ":Erroneus nickname";
  mReplies[ERR_NICKNAMEINUSE] = ":Nickname is already in use";
  mReplies[ERR_NICKCOLLISION] = ":Nickname collision KILL";
  mReplies[ERR_NOTREGISTERED] = ":You have not registered";
  mReplies[ERR_NEEDMOREPARAMS] = ":Not enough parameters";
  mReplies[ERR_ALREADYREGISTRED] = ":You may not reregister";
  mReplies[ERR_INVALIDUSERNAME] = ":Your username is not valid";
  mReplies[ERR_CHANNELISFULL] = ":Cannot join channel (channel is full)";
  mReplies[ERR_BADCHANNELKEY] = ":Cannot join channel (incorrect channel key)";
  mReplies[ERR_INVALIDCHANNELNAME] = ":Invalid channel name";
  mReplies[ERR_INVITEONLYCHAN] = ":Cannot join channel (invite only)";
  mReplies[ERR_NOSUCHCHANNEL] = ":No such channel";
  mReplies[ERR_NOTEXIST] =
      ":You must specify a parameter for the key mode. Syntax: ";
}

NumericReply::NumericReply() {}

const std::string& NumericReply::getReply(int code) { return mReplies[code]; }

NumericReply::~NumericReply() {}
