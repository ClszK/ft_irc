#include "command/PingCommand.hpp"

std::string PingCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "PING");

  return ReplyUtility::makePongReply(client, message.params[0]);
}
