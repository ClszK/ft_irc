#include "command/CommandHandler.hpp"

CommandHandler::CommandHandler() {
  mCommands["user"] = new UserCommand();
  mCommands["pass"] = new PassCommand();
  mCommands["join"] = new JoinCommand();
  mCommands["nick"] = new NickCommand();
  mCommands["mode"] = new ModeCommand();
  mCommands["privmsg"] = new PrivmsgCommand();
  mCommands["part"] = new PartCommand();
}

std::string CommandHandler::handleCommand(Client& client, Message& message) {
  if (mCommands.find(message.command) != mCommands.end()) {
    return mCommands[message.command]->execute(client, message);
  }
  return "";
}

CommandHandler::~CommandHandler() {
  for (std::map<std::string, ICommand*>::iterator it = mCommands.begin();
       it != mCommands.end(); ++it)
    delete it->second;
}