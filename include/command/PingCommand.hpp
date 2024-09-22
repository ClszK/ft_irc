#pragma once

#include "command/ICommand.hpp"

class PingCommand : public ICommand {
 public:
  std::string execute(Client& client, Message& message);
};