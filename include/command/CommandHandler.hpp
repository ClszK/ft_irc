#pragma once

#include <map>
#include <string>
#include <vector>

#include "command/Command.hpp"

class Client;
class Message;

class CommandHandler {
 public:
  CommandHandler();
  ~CommandHandler();
  std::string handleCommand(Client& client, Message& message);

 private:
  std::map<std::string, Command*> mCommands;
};