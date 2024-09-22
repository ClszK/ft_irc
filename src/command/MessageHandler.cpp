#include "command/MessageHandler.hpp"

Message MessageHandler::parseMessage(const std::string& message) {
  if (message.empty()) {
    return Message();
  }
  std::string messageCopy = message;

  size_t pos = messageCopy.find('\r');

  if (pos != std::string::npos) messageCopy.erase(pos, 1);

  std::istringstream iss(messageCopy);
  Message parsedMessage;
  std::string param;

  if (messageCopy[0] == ':') std::getline(iss, parsedMessage.prefix, ' ');

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
  os << "{prefix: " << message.prefix << ", command: " << message.command
     << ", params: [";
  for (std::vector<std::string>::const_iterator it = message.params.begin();
       it != message.params.end(); ++it) {
    os << *it << ", ";
  }
  os << "]}";
  return os;
}