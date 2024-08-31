#pragma once

#include <string>
#include <vector>

class Client;

class Command {
 public:
  virtual int execute(Client& client, std::vector<std::string>& message) = 0;

  Command() {};
  virtual ~Command() {}
};

class JoinCommand : public Command {
 public:
  virtual int execute(Client& client, std::vector<std::string>& message);
};

class PassCommand : public Command {
 public:
  virtual int execute(Client& client, std::vector<std::string>& message);
};

class NickCommand : public Command {
 public:
  virtual int execute(Client& client, std::vector<std::string>& message);
};

class UserCommand : public Command {
 public:
  virtual int execute(Client& client, std::vector<std::string>& message);
};
