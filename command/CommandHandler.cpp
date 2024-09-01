#include "CommandHandler.hpp"

CommandHandler::CommandHandler() {
  mCommands["user"] = new UserCommand();
  mCommands["pass"] = new PassCommand();
  mCommands["join"] = new JoinCommand();
  mCommands["nick"] = new NickCommand();
}

std::pair<int, std::string> CommandHandler::handleCommand(Client& client,
                                                          Message& message) {
  if (mCommands.find(message.command) != mCommands.end()) {
    return mCommands[message.command]->execute(client, message);
  }
  return std::make_pair(0, "");
}

CommandHandler::~CommandHandler() {
  for (std::map<std::string, Command*>::iterator it = mCommands.begin();
       it != mCommands.end(); ++it)
    delete it->second;
}