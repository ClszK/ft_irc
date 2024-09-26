#pragma once

#include <map>
#include <vector>

#include "command/ICommand.hpp"

class NickCommand : public ICommand {
 public:
  virtual std::string execute(Client& client, Message& message);
};