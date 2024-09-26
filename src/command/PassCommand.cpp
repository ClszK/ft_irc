#include "command/PassCommand.hpp"

std::string PassCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "PASS");
  if (client.getRegistered())
    return ReplyUtility::makeErrAlreadyRegistredReply(client);

  client.setPassword(message.params[0]);
  return "";
}
