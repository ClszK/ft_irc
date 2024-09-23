#pragma once

#include "command/ICommand.hpp"

class KickCommand : public ICommand {
 public:
  virtual std::string execute(Client& client, Message& message);
};
