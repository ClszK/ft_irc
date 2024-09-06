#include "command/MessageHandler.hpp"

#include <iostream>

Message MessageHandler::parseMessage(const std::string& message) {
  if (message.empty()) {
    return Message();
  }
  std::istringstream iss(message);
  Message parsedMessage;
  std::string param;

  if (message[0] == ':') std::getline(iss, parsedMessage.prefix, ' ');

  iss >> parsedMessage.command;

  std::transform(parsedMessage.command.begin(), parsedMessage.command.end(),
                 parsedMessage.command.begin(), ToLower());
  // 매개변수 파싱
  while (iss >> param) {
    if (param[0] == ':') {
      param.erase(0, 1);  // 콜론 제거
      std::string trailing;
      if (std::getline(iss, trailing)) {
        param += " " + trailing;  // 나머지 텍스트를 공백 포함하여 추가
      }
      std::cout << "param: " << param << "!" << std::endl;
      parsedMessage.params.push_back(param);
      break;  // 나머지 텍스트를 모두 읽었으므로 루프 종료
    }
    parsedMessage.params.push_back(param);
  }
  return parsedMessage;
}

MessageHandler::MessageHandler() {}

MessageHandler::~MessageHandler() {}

std::ostream& operator<<(std::ostream& os, const Message& message) {
  os << "Message {prefix: " << message.prefix
     << ", command: " << message.command << ", params: [";
  for (std::vector<std::string>::const_iterator it = message.params.begin();
       it != message.params.end(); ++it) {
    os << *it << ", ";
  }
  os << "]}";
  return os;
}