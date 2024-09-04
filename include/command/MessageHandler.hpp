#pragma once

#include <sstream>
#include <vector>

struct ToLower {
  char operator()(char c) const {
    return std::tolower(static_cast<unsigned char>(c));
  }
};

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

std::ostream& operator<<(std::ostream& os, const Message& message);