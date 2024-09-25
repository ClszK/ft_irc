#pragma once

#include "command/ICommand.hpp"

class QuitCommand : public ICommand {
 public:
  virtual std::string execute(Client& client, Message& message);
};