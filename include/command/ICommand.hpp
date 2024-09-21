#pragma once

#include <string>

#include "channel/Channel.hpp"
#include "client/Client.hpp"
#include "command/MessageHandler.hpp"
#include "utils/ReplyUtility.hpp"
#include "utils/StringUtility.hpp"

class ICommand {
 public:
  virtual std::string execute(Client& client, Message& message) = 0;

  virtual ~ICommand() {}
};
