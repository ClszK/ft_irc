#pragma once

#include <sstream>
#include <vector>

struct Message {
  std::string prefix;
  std::string command;
  std::vector<std::string> params;
};

class MessageHandler {
 public:
  static Message parseMessage(const std::string& message);

 private:
  MessageHandler();
  ~MessageHandler();
};