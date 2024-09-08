#pragma once

#include "command/Command.hpp"

class PassCommand : public Command {
 public:
  virtual std::string execute(Client& client, Message& message);
};
