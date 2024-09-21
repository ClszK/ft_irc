#pragma once

#include "command/ICommand.hpp"

class JoinCommand : public ICommand {
 public:
  virtual std::string execute(Client& client, Message& message);

 private:
  std::string joinKeyMode(Client& client, Channel& channel,
                          const std::string& channelName, Message& message);
};