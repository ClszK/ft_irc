#pragma once

#include "command/ICommand.hpp"

class PartCommand : public ICommand {
 public:
  virtual std::string execute(Client& client, Message& message);
};
