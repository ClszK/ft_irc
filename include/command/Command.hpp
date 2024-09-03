#pragma once

#include <string>

#include "client/Client.hpp"
#include "command/MessageHandler.hpp"
#include "utils/ReplyUtility.hpp"

class Command {
 public:
  virtual std::string execute(Client& client, Message& message) = 0;

  Command() {};
  virtual ~Command() {}
};

class JoinCommand : public Command {
 public:
  virtual std::string execute(Client& client, Message& message);
};

class PassCommand : public Command {
 public:
  virtual std::string execute(Client& client, Message& message);
};

class NickCommand : public Command {
 public:
  virtual std::string execute(Client& client, Message& message);
};

class UserCommand : public Command {
 public:
  virtual std::string execute(Client& client, Message& message);
};
