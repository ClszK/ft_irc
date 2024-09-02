#include "MessageHandler.hpp"

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
      // 콜론으로 시작하는 매개변수는 나머지 모든 텍스트를 포함
      param += ' ';
      std::string trailing;
      if (std::getline(iss, trailing)) param += trailing;
    }
    parsedMessage.params.push_back(param);
  }
  return parsedMessage;
}

MessageHandler::MessageHandler() {}

MessageHandler::~MessageHandler() {}

std::ostream& operator<<(std::ostream& os, const Message& message) {
  os << "Message{prefix: " << message.prefix << ", command: " << message.command
     << ", params: [";
  for (std::vector<std::string>::const_iterator it = message.params.begin();
       it != message.params.end(); ++it) {
    os << *it << ", ";
  }
  os << "]}";
  return os;
}