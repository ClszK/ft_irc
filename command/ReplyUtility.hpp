#pragma once

#include "NumericReply.hpp"

class ReplyUtility {
 public:
  static ReplyPair makeWelcomeReply(const std::string& nickName);
  static ReplyPair makeNeedMoreParamsReply(const std::string& command);
  static ReplyPair makeErrNonicknameGivenReply(const std::string& str);
  static ReplyPair makeErrorReply();
  static ReplyPair makeNotRegisteredReply(const std::string& command);

  static bool isValidChannelName(const std::string& channelName);
  static bool isValidNickName(const std::string& nickName);
};