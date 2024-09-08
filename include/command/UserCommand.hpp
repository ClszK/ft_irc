#pragma once

#include "command/ICommand.hpp"

class UserCommand : public ICommand {
 public:
  virtual std::string execute(Client& client, Message& message);
};