#include "command/WhoCommand.hpp"

std::string WhoCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "WHO");

  std::string channelName = message.params[0];

  Channel* channel = Channel::findChannel(channelName);
  if (channel == NULL)
    return ReplyUtility::makeErrNoSuchChannelReply(client, channelName);

  std::string replyStr = "";

  const std::vector<Client*> userList = channel->getUserList();

  for (std::vector<Client*>::const_iterator it = userList.cbegin();
       it != userList.cend(); ++it) {
    replyStr += ReplyUtility::makeWhoReply(client, *it, channelName);
  }

  replyStr += ReplyUtility::makeWhoEndReply(client, channelName);
  return replyStr;
}