#pragma once

#include <vector>

#include "command/ICommand.hpp"

class WhoCommand : public ICommand {
 public:
  std::string execute(Client& client, Message& message);
};