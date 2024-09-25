#include "command/UserCommand.hpp"

/**
 * user <username> <hostname> <servername> <realname>
 */
std::string UserCommand::execute(Client& client, Message& message) {
  std::string nickName = client.getNickName();

  if (nickName == "") nickName = "*";
  if (message.params.size() < 4)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "USER", nickName);

  std::string userName = message.params[0];
  std::string realName = message.params[3];
  if (client.getUserName() != "")
    return ReplyUtility::makeErrAlreadyRegistredReply(client);

  if (!StringUtility::isValidUserName(userName))
    return ReplyUtility::makeErrInvalidUserNameReply(client, "USER");

  client.setUserName(message.params[0]);
  client.setRealName(message.params[3]);
  if (client.getNickName() != "") {
    if (client.isPasswordValid())
      return ReplyUtility::makeSuccessConnectReply(client);

    return ReplyUtility::makeErrorReply(client,
                                        "Access denied by configuration");
  }
  return "";
}
