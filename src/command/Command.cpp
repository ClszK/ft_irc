#include "command/Command.hpp"

/**
 * :irc.local 461 test1 JOIN :Not enough parameters.
 */
std::string JoinCommand::execute(Client& client, Message& message) {
  (void)client;
  if (message.params.size() < 1)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "JOIN");

  if (StringUtility::isValidChannelName(message.params[0]))
    // return ReplyUtility::makeErr

    return "";
}

std::string PassCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "PASS");

  client.setPassword(message.params[0]);
  return "";
}

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
