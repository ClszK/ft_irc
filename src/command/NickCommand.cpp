#include "command/NickCommand.hpp"

std::string NickCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "NICK");

  if (!client.setNickName(message.params[0]))
    return ReplyUtility::makeErrNonicknameGivenReply(client, message.params[0]);

  if (client.getUserName() != "") {
    if (client.isPasswordValid())
      return ReplyUtility::makeSuccessConnectReply(client);

    return ReplyUtility::makeErrorReply(client);
  }
  return "";
}
