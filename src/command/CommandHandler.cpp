#include "command/CommandHandler.hpp"

CommandHandler::CommandHandler() {
  mCommands["user"] = new UserCommand();
  mCommands["pass"] = new PassCommand();
  mCommands["join"] = new JoinCommand();
  mCommands["nick"] = new NickCommand();
  mCommands["mode"] = new ModeCommand();
  mCommands["privmsg"] = new PrivmsgCommand();
  mCommands["part"] = new PartCommand();
  mCommands["ping"] = new PingCommand();
  mCommands["who"] = new WhoCommand();
  mCommands["invite"] = new InviteCommand();
  mCommands["kick"] = new KickCommand();
  mCommands["topic"] = new TopicCommand();
}

std::string CommandHandler::handleCommand(Client& client, Message& message) {
  if (mCommands.find(message.command) != mCommands.end()) {
    return mCommands[message.command]->execute(client, message);
  }
  return ReplyUtility::makeErrUnknownCommandReply(client, message.command);
}

CommandHandler::~CommandHandler() {
  for (std::map<std::string, ICommand*>::iterator it = mCommands.begin();
       it != mCommands.end(); ++it)
    delete it->second;
}