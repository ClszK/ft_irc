#pragma once

#include "../Client.hpp"
#include "../MessageHandler.hpp"
#include "ReplyUtility.hpp"
class Command {
 public:
  virtual ReplyPair execute(Client& client, Message& message) = 0;

  Command() {};
  virtual ~Command() {}

 protected:
  ReplyPair makeNeedMoreParamsReply(const std::string& command);
};

class JoinCommand : public Command {
 public:
  virtual ReplyPair execute(Client& client, Message& message);
};

class PassCommand : public Command {
 public:
  virtual ReplyPair execute(Client& client, Message& message);
};

class NickCommand : public Command {
 public:
  virtual ReplyPair execute(Client& client, Message& message);
};

class UserCommand : public Command {
 public:
  virtual ReplyPair execute(Client& client, Message& message);
};
