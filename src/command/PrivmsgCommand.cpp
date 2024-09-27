#include "command/PrivmsgCommand.hpp"

/**
 * @TODO: PRIVMSG 명령어를 처리하는 함수
 * - 채널 모드가 -n이면 외부에서도 채널에 메시지를 보낼 수 있게 변경해야함.
 */
std::string PrivmsgCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 2)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "PRIVMSG");

  if (!client.getRegistered())
    return ReplyUtility::makeErrNotRegisteredReply(client, "PRIVMSG");

  std::string msgTarget = message.params[0];
  std::string messageContent = message.params[1];

  for (size_t i = 2; i < message.params.size(); i++) {
    messageContent += " " + message.params[i];
  }

  if (StringUtility::isValidChannelName(msgTarget)) {
    Channel* target = Channel::findChannel(msgTarget);
    if (target == NULL)
      return ReplyUtility::makeErrNoSuchChannelReply(client, msgTarget);
    if (target->getChannelMode().find('n') != std::string::npos &&
        !target->isUserInChannel(client.getNickName()))
      return ReplyUtility::makeErrCannotSendToChanReply(client, msgTarget);
    std::string response =
        ReplyUtility::makePrivmsgReply(client, msgTarget, messageContent);
    target->sendPrivmsg(client, response);
  } else {
    Client* target = Client::findClient(msgTarget);
    if (target == NULL)
      return ReplyUtility::makeErrNoSuchNickReply(client, msgTarget);
    std::string response =
        ReplyUtility::makePrivmsgReply(client, msgTarget, messageContent);
    target->sendPrivmsg(response);
  }

  return "";
}