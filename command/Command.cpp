#include "Command.hpp"

int JoinCommand::execute(Client& client, std::vector<std::string>& message) {
  return 0;
}

int PassCommand::execute(Client& client, std::vector<std::string>& message) {}

int NickCommand::execute(Client& client, std::vector<std::string>& message) {}

int UserCommand::execute(Client& client, std::vector<std::string>& message) {}