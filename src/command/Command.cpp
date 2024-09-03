#include "command/Command.hpp"

std::string JoinCommand::execute(Client& client, Message& message) {
  (void)client;
  if (message.params.size() < 1) {
  }
  return "";
}

std::string PassCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeNeedMoreParamsReply("PASS");

  client.setPassword(message.params[0]);
  return "";
}

std::string NickCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeNeedMoreParamsReply("NICK");

  if (!client.setNickName(message.params[0]))
    return ReplyUtility::makeErrNonicknameGivenReply(message.params[0]);

  if (client.getUserName() != "") {
    if (client.isPasswordValid())
      return ReplyUtility::makeWelcomeReply(message.params[0]);

    return ReplyUtility::makeErrorReply();
  }
  return "";
}

std::string UserCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 4)
    return ReplyUtility::makeNeedMoreParamsReply("USER");

  if (!client.setUserName(message.params[0]))
    return ReplyUtility::makeErrorReply();
  client.setRealName(message.params[3]);
  if (client.getNickName() != "") {
    if (client.isPasswordValid())
      return ReplyUtility::makeWelcomeReply(message.params[0]);

    return ReplyUtility::makeErrorReply();
  }
  return "";
}

bool setNickName(const std::string& nickName);