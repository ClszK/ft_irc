#include "command/InviteCommand.hpp"

std::string InviteCommand::execute(Client &client, Message &message) {
  if (message.params.size() < 2)
    return ReplyUtility::makeEndofInvitelstReply(client);

  if (!client.getRegistered())
    return ReplyUtility::makeErrNotRegisteredReply(client, "INVITE");

  std::string channelName = StringUtility::parseComma(message.params[1]);
  Channel *channel = Channel::findChannel(channelName);
  if (!StringUtility::isValidChannelName(channelName) || channel == NULL)
    return ReplyUtility::makeErrNoSuchChannelReply(client, channelName);

  std::string nickName = StringUtility::parseComma(message.params[0]);
  Client *target = Client::findClient(nickName);
  if (target == NULL)
    return ReplyUtility::makeErrNoSuchNickReply(client, nickName);

  if (!channel->isUserInChannel(client.getNickName()))
    return ReplyUtility::makeErrUserNotOnChannelReply(client, channelName);

  if (channel->isUserInChannel(nickName))
    return ReplyUtility::makeErrUserOnChannelReply(client, nickName,
                                                   channelName);

  if (!channel->isOperator(client))
    return ReplyUtility::makeErrInviteOPrivsNeededReply(client, channelName);

  // 채널에 초대
  client.sendPrivmsg(
      ReplyUtility::makeInvitingReply(client, nickName, channelName));

  std::vector<std::string> params;
  params.push_back(nickName);
  params.push_back(channelName);

  target->sendPrivmsg(ReplyUtility::makeCommandReply(client, "INVITE", params));
  channel->setInvitedListAdd(*target);

  return "";
}