#pragma once

#include <map>
#include <string>
#include <vector>

#include "Command.hpp"

class CommandHandler {
 public:
  CommandHandler();
  ~CommandHandler();
  ReplyPair handleCommand(Client& client, Message& message);

 private:
  std::map<std::string, Command*> mCommands;
  std::string mPassword;
};