#include "command/PartCommand.hpp"

#include "server/Server.hpp"

std::string PartCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "PART");

  std::string replyStr = "";
  while (message.params[0].size()) {
    std::string channelName = StringUtility::parseComma(message.params[0]);

    if (!StringUtility::isValidChannelName(channelName)) {
      replyStr += ReplyUtility::makeErrNoSuchChannelReply(client, channelName);
      continue;
    }

    Channel* channel = Channel::findChannel(channelName);
    if (channel == NULL) {
      replyStr += ReplyUtility::makeErrNoSuchChannelReply(client, channelName);
      continue;
    }

    if (!channel->isUserInChannel(client.getNickName())) {
      replyStr += ReplyUtility::makeErrNotInChannelReply(client, channelName);
      continue;
    }

    std::string partMessage = "";
    if (message.params.size() > 1) {
      partMessage = message.params[1];
      for (size_t i = 2; i < message.params.size(); i++) {
        partMessage += " " + message.params[i];
      }
    }

    std::string response =
        ReplyUtility::makePartReply(client, channelName, partMessage);
    channel->sendPart(client, response);
    channel->removeUser(client);
    client.removeChannel(channelName);

    if (channel->isEmpty()) {
      Server::getInstance()->removeChannel(channelName);
    }
  }

  return replyStr;
}