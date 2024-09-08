#pragma once

#include "command/Command.hpp"

class NickCommand : public Command {
 public:
  virtual std::string execute(Client& client, Message& message);
};