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
 */
bool StringUtility::isValidUserName(const std::string& userName) {
  if (userName.length() < 1 || userName.length() > USERLEN) return false;

  for (size_t i = 0; i < userName.length(); i++) {
    if (std::string(" \0\r\n@").find(userName[i]) != std::string::npos)
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
  if (channelName.length() < 3 || channelName.length() > CHANNELLEN)
    return false;

  if (std::string("#&+!").find(channelName[0]) != std::string::npos)
    return false;

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
bool StringUtility::isValidKey(const std::string& key) {
  if (key.length() < 1 || key.length() > KEYLEN) return false;

  for (size_t i = 0; i < key.length(); i++) {
    if (std::string("\0\r\n\a\f\t\v ").find(key[i]) != std::string::npos)
      return false;
  }

  return true;
}