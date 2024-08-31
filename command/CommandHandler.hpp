#pragma once

#include <map>
#include <string>
#include <vector>

#include "Command.hpp"

class Client;

class CommandHandler {
 public:
  CommandHandler();
  ~CommandHandler();
  int handleCommand(Client& client,
                    std::pair<std::string, std::vector<std::string> >& message);

 private:
  std::map<std::string, Command*> mCommands;
  std::string mPassword;
};