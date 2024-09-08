#pragma once

#include "command/Command.hpp"

class JoinCommand : public Command {
 public:
  virtual std::string execute(Client& client, Message& message);
};