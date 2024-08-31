#include "Command.hpp"

void JoinCommand::execute(Client& client, std::vector<std::string>& message) {}

void PassCommand::execute(Client& client, std::vector<std::string>& message) {
  if (message.size() < 1) {
    client.sendNumericReply(ERR_NEEDMOREPARAMS);
  }
}

void NickCommand::execute(Client& client, std::vector<std::string>& message) {}

void UserCommand::execute(Client& client, std::vector<std::string>& message) {}
