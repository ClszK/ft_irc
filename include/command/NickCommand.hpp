#pragma once

#include "command/ICommand.hpp"

class NickCommand : public ICommand {
 public:
  virtual std::string execute(Client& client, Message& message);
};