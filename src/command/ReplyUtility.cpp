#include "utils/ReplyUtility.hpp"

std::string ReplyUtility::makeWelcomeReply(const std::string& nickName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << RPL_WELCOME << " " << nickName
     << " " << NumericReply::getReply(RPL_WELCOME) << "\n";
  return std::make_pair(
      static_cast<int>(RPL_WELCOME),
      "* " + nickName + " " + NumericReply::getReply(RPL_WELCOME));
}

std::string ReplyUtility::makeNeedMoreParamsReply(const std::string& command) {
  return std::make_pair(
      static_cast<int>(ERR_NEEDMOREPARAMS),
      "* " + command + " " + NumericReply::getReply(ERR_NEEDMOREPARAMS));
}

std::string ReplyUtility::makeErrNonicknameGivenReply(const std::string& str) {
  return std::make_pair(
      static_cast<int>(ERR_NONICKNAMEGIVEN),
      "* " + str + " " + NumericReply::getReply(ERR_NONICKNAMEGIVEN));
}

std::string ReplyUtility::makeErrorReply() {
  return std::make_pair(-1,
                        "임시 ERROR :Closing link: (123@172.29.0.1) [Access "
                        "denied by configuration]");
}
std::string ReplyUtility::makeNotRegisteredReply(Client& client,
                                                 const std::string& command) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_NOTREGISTERED << " "
     << client.getNickName() << " " << command << " "
     << NumericReply::getReply(ERR_NOTREGISTERED) << "\n";
  return std::make_pair(
      static_cast<int>(ERR_NOTREGISTERED),
      "* " + command + " " + NumericReply::getReply(ERR_NOTREGISTERED));
}

std::string ReplyUtility::makeInvalidUserNameReply(Client& client,
                                                   const std::string& command) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_INVALIDUSERNAME << " "
     << client.getNickName() << " " << command << " "
     << NumericReply::getReply(ERR_INVALIDUSERNAME) << "\n";

  return ss.str();
}
