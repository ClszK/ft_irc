#include "ReplyUtility.hpp"

ReplyPair ReplyUtility::makeWelcomeReply(const std::string& nickName) {
  return std::make_pair(
      static_cast<int>(RPL_WELCOME),
      "* " + nickName + " " + NumericReply::getReply(RPL_WELCOME));
}

ReplyPair ReplyUtility::makeNeedMoreParamsReply(const std::string& command) {
  return std::make_pair(
      static_cast<int>(ERR_NEEDMOREPARAMS),
      "* " + command + " " + NumericReply::getReply(ERR_NEEDMOREPARAMS));
}

ReplyPair ReplyUtility::makeErrNonicknameGivenReply(const std::string& str) {
  return std::make_pair(
      static_cast<int>(ERR_NONICKNAMEGIVEN),
      "* " + str + " " + NumericReply::getReply(ERR_NONICKNAMEGIVEN));
}

ReplyPair ReplyUtility::makeErrorReply() {
  return std::make_pair(-1,
                        "임시 ERROR :Closing link: (123@172.29.0.1) [Access "
                        "denied by configuration]");
}

bool ReplyUtility::isValidChannelName(const std::string& channelName) {
  if (channelName.length() < 2 || channelName.length() > 50) return false;

  if (std::string("#&+!").find(channelName[0]) == std::string::npos)
    return false;

  for (size_t i = 1; i < channelName.length(); i++) {
    if (!std::isalnum(channelName[i]) &&
        std::string("-_.,").find(channelName[i]) == std::string::npos) {
      return false;
    }
  }

  return true;
}

bool ReplyUtility::isValidNickName(const std::string& nickName) {
  if (nickName.length() < 1 || nickName.length() > 9) return false;

  if (!std::isalpha(nickName[0]) &&
      std::string("[\\]^_{|}").find(nickName[0]) == std::string::npos)
    return false;

  for (size_t i = 1; i < nickName.length(); i++) {
    if (!std::isalnum(nickName[i]) &&
        std::string("[\\]^_{|}").find(nickName[i]) == std::string::npos) {
      return false;
    }
  }

  return true;
}