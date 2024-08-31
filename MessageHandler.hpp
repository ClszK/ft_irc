#pragma once

#include <sstream>
#include <vector>

#include "Client.hpp"

class MessageHandler {
 public:
  static std::pair<std::string, std::vector<std::string>> parseMessage(
      const std::string& message);

 private:
  MessageHandler();
  ~MessageHandler();
};