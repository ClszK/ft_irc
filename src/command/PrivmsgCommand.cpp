#include "command/PrivmsgCommand.hpp"

std::string PrivmsgCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 2)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "PRIVMSG");

  std::string msgTarget = message.params[0];
  std::string messageContent = message.params[1];

  for (size_t i = 2; i < message.params.size(); i++) {
    messageContent += " " + message.params[i];
  }

  if (StringUtility::isValidChannelName(msgTarget)) {
    Channel* target = Channel::findChannel(msgTarget);
    if (target == NULL)
      return ReplyUtility::makeErrNoSuchChannelReply(client, msgTarget);
    if (!target->isUserInChannel(client.getNickName()))
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

  //   AChannel* target;

  //   if (StringUtility::isValidChannelName(msgTarget)) {
  //     target = Channel::findChannel(msgTarget);
  //     if (target == NULL)
  //       return ReplyUtility::makeErrNoSuchChannelReply(client, msgTarget);

  //   } else {
  //     target = Client::findClient(msgTarget);
  //     if (target == NULL)
  //       return ReplyUtility::makeErrNoSuchNickReply(client, msgTarget);
  //   }

  //   std::string response =
  //       ReplyUtility::makePrivmsgReply(client, targetName, messageContent);
  //   target->send(response);

  return "";
}