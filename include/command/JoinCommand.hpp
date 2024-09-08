#pragma once

#include "command/ICommand.hpp"

class JoinCommand : public ICommand {
 public:
  virtual std::string execute(Client& client, Message& message);
};