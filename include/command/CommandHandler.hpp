#pragma once

#include <map>
#include <string>
#include <vector>

#include "command/JoinCommand.hpp"
#include "command/ModeCommand.hpp"
#include "command/NickCommand.hpp"
#include "command/PassCommand.hpp"
#include "command/PrivmsgCommand.hpp"
#include "command/UserCommand.hpp"

class CommandHandler {
 public:
  std::string handleCommand(Client& client, Message& message);

  static CommandHandler* getInstance() {
    static CommandHandler instance;
    return &instance;
  }

 private:
  std::map<std::string, ICommand*> mCommands;

  CommandHandler();
  ~CommandHandler();
};