#pragma once

#include <set>

#include "command/ICommand.hpp"

class ModeCommand : public ICommand {
 public:
  virtual std::string execute(Client& client, Message& message);
};