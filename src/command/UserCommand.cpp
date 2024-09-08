#include "command/UserCommand.hpp"

std::string UserCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 4)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "USER");

  if (!client.setUserName(message.params[0]))
    return ReplyUtility::makeErrorReply(client);
  client.setRealName(message.params[3]);
  if (client.getNickName() != "") {
    if (client.isPasswordValid())
      return ReplyUtility::makeSuccessConnectReply(client);

    return ReplyUtility::makeErrorReply(client);
  }
  return "";
}
