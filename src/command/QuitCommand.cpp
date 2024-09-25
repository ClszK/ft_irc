#include "command/QuitCommand.hpp"

std::string QuitCommand::execute(Client& client, Message& message) {
  std::string reply = "";

  if (message.params.size()) {
    std::string paramStr = "Quit: ";

    for (size_t i = 0; i < message.params.size(); i++)
      paramStr += message.params[i] + " ";

    reply = ReplyUtility::makeErrorReply(client, paramStr);
  } else
    reply = ReplyUtility::makeErrorReply(client, "Client exited");

  return reply;
}