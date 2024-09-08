#include "command/PassCommand.hpp"

std::string PassCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "PASS");

  client.setPassword(message.params[0]);
  return "";
}
