#include "command/QuitCommand.hpp"

std::string QuitCommand::execute(Client& client, Message& message) {
  std::string paramStr = "Client exited";
  std::string reply = "";

  if (message.params.size()) {
    paramStr = "Quit: ";

    for (size_t i = 0; i < message.params.size(); i++)
      paramStr += message.params[i] + " ";

    reply = ReplyUtility::makeErrorReply(client, paramStr);
  } else
    reply = ReplyUtility::makeErrorReply(client, "Client exited");

  Server* server = Server::getInstance();
  std::map<std::string, Channel*> channels = server->getChannels();
  std::vector<std::string> params;

  params.push_back(paramStr);
  for (std::map<std::string, Channel*>::const_iterator it = channels.cbegin();
       it != channels.cend(); ++it) {
    it->second->sendPrivmsg(
        client, ReplyUtility::makeCommandReply(client, "QUIT", params));
    it->second->removeUser(client);
    client.removeChannel(it->second);
    if (it->second->isEmpty()) server->removeChannel(it->first);
  }
  return reply;
}