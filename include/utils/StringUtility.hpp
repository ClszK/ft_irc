#pragma once

#include <set>
#include <sstream>
#include <string>

#include "server/IRCConstants.hpp"

class StringUtility {
 private:
  static bool isSpecial(char c);

 public:
  static bool isValidNickName(const std::string& nickName);
  static bool isValidUserName(std::string& userName);
  static bool isValidChannelName(const std::string& channelName);
  static bool isValidChannelKey(const std::string& key);
  static std::string removeDuplicateChars(const std::string& input);
  static std::string parseComma(std::string& str);
  static std::string numberToString(long number);
};