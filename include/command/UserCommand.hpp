#pragma once

#include "command/Command.hpp"

class UserCommand : public Command {
 public:
  virtual std::string execute(Client& client, Message& message);
};