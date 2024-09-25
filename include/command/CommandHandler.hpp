#pragma once

#include <map>
#include <string>
#include <vector>

#include "command/InviteCommand.hpp"
#include "command/JoinCommand.hpp"
#include "command/KickCommand.hpp"
#include "command/ModeCommand.hpp"
#include "command/NickCommand.hpp"
#include "command/PartCommand.hpp"
#include "command/PassCommand.hpp"
#include "command/PingCommand.hpp"
#include "command/PrivmsgCommand.hpp"
#include "command/QuitCommand.hpp"
#include "command/TopicCommand.hpp"
#include "command/UserCommand.hpp"
#include "command/WhoCommand.hpp"

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