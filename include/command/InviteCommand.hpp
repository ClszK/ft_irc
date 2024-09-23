#pragma once

#include "command/ICommand.hpp"

class InviteCommand : public ICommand {
 public:
  virtual std::string execute(Client& client, Message& message);
};