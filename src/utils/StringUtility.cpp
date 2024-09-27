#include "utils/StringUtility.hpp"

/**
 * "[", "]", "\", "`", "_", "^", "{", "|", "}"
 *
 * %x5B-60 / %x7B-7D
 */
bool StringUtility::isSpecial(char c) {
  return (c >= 0x5B && c <= 0x60) || (c >= 0x7B && c <= 0x7D);
}

/**
 * ( letter / special ) *8( letter / digit / special / "-" )
 */
bool StringUtility::isValidNickName(const std::string& nickName) {
  if (nickName.length() < 1 || nickName.length() > NICKLEN) return false;

  if (!(std::isalpha(nickName[0]) || isSpecial(nickName[0]))) return false;

  for (size_t i = 1; i < nickName.length(); i++) {
    if (!(std::isalnum(nickName[i]) || nickName[i] == '-' ||
          isSpecial(nickName[i])))
      return false;
  }

  return true;
}

/**
 * 허용되지 않는 ASCII 문자
 *
 * NUL (\0): ASCII 0x00
 * CR (\r): ASCII 0x0D (캐리지 리턴)
 * LF (\n): ASCII 0x0A (라인 피드)
 * Space ( ): ASCII 0x20 (공백 문자)
 * At (@): ASCII 0x40 (앳 기호)
 * 이 부분은 RFC 2812 기준.
 */
bool StringUtility::isValidUserName(std::string& userName) {
  if (userName.length() > USERLEN) userName = userName.substr(0, USERLEN);

  for (size_t i = 0; i < userName.length(); ++i) {
    char c = userName[i];

    if (std::isalnum(c))
      continue;
    else if (isSpecial(c))
      continue;
    else
      return false;
  }

  return true;
}

/**
 * channel    =  ( "#" / "+" / ( "!" channelid ) / "&" ) chanstring
 *               [ ":" chanstring ]
 * chanstring =  %x01-07 / %x08-09 / %x0B-0C / %x0E-1F / %x21-2B
 * chanstring =/ %x2D-39 / %x3B-FF
                 ; any octet except NUL, BELL, CR, LF, " ", "," and ":"
 * channelid는 서버에서 부여되는 거 같음. 그리고 서버 간의 충돌을 방지하기 위해
        넣는데
 * 현재 단일 서버 구현이기 때문에 구현할 필요 없음.
 * [:chanstring]은 고급 IRC 서버 기능으로 구현 안함.
 */
bool StringUtility::isValidChannelName(const std::string& channelName) {
  if (channelName.length() < 1 || channelName.length() > CHANNELLEN)
    return false;

  if (channelName[0] != '#') return false;

  for (size_t i = 1; i < channelName.length(); i++) {
    if (std::string(" ,:\r\n\0\a").find(channelName[i]) != std::string::npos)
      return false;
  }
  return true;
}

/**
 *   key        =  1*23( %x01-05 / %x07-08 / %x0C / %x0E-1F / %x21-7F )
                  ; any 7-bit US_ASCII character,
                  ; except NUL, CR, LF, FF, h/v TABs, and " "
 */
bool StringUtility::isValidChannelKey(const std::string& key) {
  if (key.length() < 1 || key.length() > KEYLEN) return false;

  for (size_t i = 0; i < key.length(); i++) {
    if (std::string("\0\r\n\a\f\t\v ").find(key[i]) != std::string::npos)
      return false;
  }

  return true;
}

std::string StringUtility::removeDuplicateChars(const std::string& input) {
  std::set<char> seen;
  std::string result;
  for (std::string::const_iterator it = input.begin(); it != input.end();
       ++it) {
    if (seen.find(*it) == seen.end()) {
      seen.insert(*it);
      result += *it;
    }
  }
  return result;
}

#include <iostream>

std::string StringUtility::parseComma(std::string& str) {
  std::string result = "";

  size_t pos = str.find(",");
  if (pos == std::string::npos) pos = str.size();
  result = str.substr(0, pos);
  str.erase(0, pos + (pos != str.size()));
  return result;
}