#pragma once

#include "../Client.hpp"
#include "../MessageHandler.hpp"
#include "NumericReply.hpp"

class Command {
 public:
  virtual std::pair<int, std::string> execute(Client& client,
                                              Message& message) = 0;

  Command() {};
  virtual ~Command() {}

 protected:
  NumericReply mNumericReply;
};

class JoinCommand : public Command {
 public:
  virtual std::pair<int, std::string> execute(Client& client, Message& message);
};

class PassCommand : public Command {
 public:
  virtual std::pair<int, std::string> execute(Client& client, Message& message);
};

class NickCommand : public Command {
 public:
  virtual std::pair<int, std::string> execute(Client& client, Message& message);

 private:
  bool isValidNickName(const std::string& nickName);
};

class UserCommand : public Command {
 public:
  virtual std::pair<int, std::string> execute(Client& client, Message& message);
};
