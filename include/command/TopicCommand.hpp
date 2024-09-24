#pragma once

#include "command/ICommand.hpp"

class TopicCommand : public ICommand {
 public:
  virtual std::string execute(Client& client, Message& message);
};