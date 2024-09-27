#include "command/KickCommand.hpp"

#include "server/Server.hpp"

std::string KickCommand::execute(Client &client, Message &message) {
  if (message.params.size() < 2)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "KICK");

  if (!client.getRegistered())
    return ReplyUtility::makeErrNotRegisteredReply(client, "KICK");

  std::string replyStr = "";
  while (message.params[0].size()) {
    std::string channelName = StringUtility::parseComma(message.params[0]);

    if (!StringUtility::isValidChannelName(channelName)) {
      replyStr += ReplyUtility::makeErrNoSuchChannelReply(client, channelName);
      continue;
    }

    Channel *channel = Channel::findChannel(channelName);
    if (channel == NULL) {
      replyStr += ReplyUtility::makeErrNoSuchChannelReply(client, channelName);
      continue;
    }

    if (!channel->isUserInChannel(client.getNickName())) {
      replyStr += ReplyUtility::makeErrNotInChannelReply(client, channelName);
      continue;
    }

    if (!channel->isOperator(client)) {
      replyStr += ReplyUtility::makeErrChanOPrivsNeededReply(
          client, channelName, "kick");
      continue;
    }

    std::string kickNick = StringUtility::parseComma(message.params[1]);
    Client *target = Client::findClient(kickNick);
    if (target == NULL) {
      replyStr += ReplyUtility::makeErrNoSuchNickReply(client, kickNick);
      continue;
    }

    if (!channel->isUserInChannel(kickNick)) {
      replyStr += ReplyUtility::makeErrUserNotInChannelReply(client, kickNick,
                                                             channelName);
      continue;
    }

    std::string kickMessage = "";
    if (message.params.size() > 2) {
      kickMessage = message.params[2];
      for (size_t i = 3; i < message.params.size(); i++) {
        kickMessage += " " + message.params[i];
      }
    }

    std::string response =
        ReplyUtility::makeKickReply(client, channelName, kickNick, kickMessage);

    channel->sendPart(response);
    channel->removeUser(*target);
    client.removeChannel(channel);

    if (channel->isEmpty()) Server::getInstance()->removeChannel(channelName);
  }

  return replyStr;
}
