#include "Command.hpp"

ReplyPair JoinCommand::execute(Client& client, Message& message) {
  (void)client;
  if (message.params.size() < 1) {
  }
  return std::make_pair(0, "");
}

/*
#, &, +, 또는 !로 시작해야 합니다.
영숫자와 몇 가지 특수 문자(-, _, ., ,)를 사용할 수 있습니다.
최대 길이는 50자입니다.
채널명은 대소문자를 구분하지 않습니다.
공백이나 특정 특수 문자(: 등)는 사용할 수 없습니다.
*/

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
