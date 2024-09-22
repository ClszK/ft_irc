#include "command/NickCommand.hpp"

std::string NickCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "NICK");

  if (StringUtility::isValidNickName(message.params[0]) == false)
    return ReplyUtility::makeErrErroneusNickNameReply(client,
                                                      message.params[0]);

  if (Client::findClient(message.params[0]) != NULL)
    return ReplyUtility::makeErrAlreadyNickUseReply(client, message.params[0]);

  client.setNickName(message.params[0]);

  if (client.getUserName() != "") {
    if (client.isPasswordValid())
      return ReplyUtility::makeSuccessConnectReply(client);

    return ReplyUtility::makeErrorReply(client);
  }
  return "";
}
