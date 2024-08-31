#include "CommandHandler.hpp"

CommandHandler::CommandHandler() {
  mCommands["user"] = new UserCommand();
  mCommands["pass"] = new PassCommand();
  mCommands["join"] = new JoinCommand();
  mCommands["nick"] = new NickCommand();
}
int CommandHandler::handleCommand(
    Client& client,
    std::pair<std::string, std::vector<std::string> >& message) {
  if (mCommands.find(message.first) != mCommands.end()) {
    return mCommands[message.first]->execute(client, message.second);
  }
  return -1;
}

CommandHandler::~CommandHandler() {
  for (std::map<std::string, Command*>::iterator it = mCommands.begin();
       it != mCommands.end(); ++it)
    delete it->second;
}