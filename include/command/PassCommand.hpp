#pragma once

#include "command/ICommand.hpp"

class PassCommand : public ICommand {
 public:
  virtual std::string execute(Client& client, Message& message);
};
