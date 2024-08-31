#include "MessageHandler.hpp"

std::pair<std::string, std::vector<std::string>> MessageHandler::parseMessage(
    const std::string& message) {
  std::istringstream iss(message);
  std::string command;
  std::vector<std::string> params;
  std::string param;

  // 명령어 파싱
  iss >> command;

  // 매개변수 파싱
  while (iss >> param) {
    if (param[0] == ':') {
      // 콜론으로 시작하는 매개변수는 나머지 모든 텍스트를 포함
      param += ' ';
      std::string trailing;
      if (std::getline(iss, trailing)) {
        param += trailing;
      }
    }
    params.push_back(param);
  }

  return {command, params};
}
MessageHandler::MessageHandler() {}

MessageHandler::~MessageHandler() {}