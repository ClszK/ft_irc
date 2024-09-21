#pragma once

#include "command/ICommand.hpp"

class PrivmsgCommand : public ICommand {
 public:
  PrivmsgCommand() {}
  ~PrivmsgCommand() {}

  virtual std::string execute(Client& client, Message& message);
};