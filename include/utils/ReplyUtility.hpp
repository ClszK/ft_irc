#pragma once

#include <sstream>
#include <string>

#include "client/Client.hpp"
#include "utils/NumericReply.hpp"

class ReplyUtility {
 public:
  static std::string makeWelcomeReply(const std::string& nickName);
  static std::string makeNeedMoreParamsReply(const std::string& command);
  static std::string makeErrNonicknameGivenReply(const std::string& str);
  static std::string makeErrorReply();
  static std::string makeNotRegisteredReply(Client& client,
                                            const std::string& command);
  static std::string makeInvalidUserNameReply(Client& client,
                                              const std::string& command);
};