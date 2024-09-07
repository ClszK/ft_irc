#pragma once

#include <map>
#include <string>
#include <vector>

#include "command/Command.hpp"

class CommandHandler {
 public:
  std::string handleCommand(Client& client, Message& message);

  static CommandHandler* getInstance() {
    static CommandHandler instance;
    return &instance;
  }

 private:
  std::map<std::string, Command*> mCommands;

  CommandHandler();
  ~CommandHandler();
};