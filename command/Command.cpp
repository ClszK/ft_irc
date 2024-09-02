#include "Command.hpp"

ReplyPair JoinCommand::execute(Client& client, Message& message) {
  (void)client;
  if (message.params.size() < 1) {
  }
  return std::make_pair(0, "");
}

ReplyPair PassCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeNeedMoreParamsReply("PASS");

  client.setPassword(message.params[0]);
  return std::make_pair(0, "");
}

ReplyPair NickCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeNeedMoreParamsReply("NICK");

  if (!ReplyUtility::isValidNickName(message.params[0]))
    return ReplyUtility::makeErrNonicknameGivenReply(message.params[0]);

  client.setNickName(message.params[0]);
  if (client.getUserName() != "") {
    if (client.isPasswordValid(client.getPassword()))
      return ReplyUtility::makeWelcomeReply(message.params[0]);

    return ReplyUtility::makeErrorReply();
  }
  return std::make_pair(0, "");
}

ReplyPair UserCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 4)
    return ReplyUtility::makeNeedMoreParamsReply("USER");

  client.setUserName(message.params[0]);
  client.setRealName(message.params[3]);
  if (client.getNickName() != "") {
    if (client.isPasswordValid(client.getPassword()))
      return ReplyUtility::makeWelcomeReply(message.params[0]);

    return ReplyUtility::makeErrorReply();
  }
  return std::make_pair(0, "");
}
