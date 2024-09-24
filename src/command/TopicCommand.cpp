#include "command/TopicCommand.hpp"

std::string TopicCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "TOPIC");

  std::string channelName = message.params[0];
  if (message.params.size() == 1) {
    Channel* channel = Channel::findChannel(channelName);
    if (channel == NULL)
      return ReplyUtility::makeErrNoSuchChannelReply(client, channelName);

    std::string topic = channel->getTopic();
    if (topic.empty())
      return ReplyUtility::makeNoTopicReply(client, channelName);
  }

  return "";
}
