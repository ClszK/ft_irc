#include "command/TopicCommand.hpp"

/**
 * TopicCommand
 *
 * 채널명만 있는 경우. 해당 채널의 토픽을 반환한다.
 * :irc.local 332 a #123 :12
 * :irc.local 333 a #123 t5!root@192.168.65.1 :1727197400
 *
 * 채널에 참가하지 않은 상태에서 토픽을 변경하려고 할 경우.
 * :irc.local 442 b #123 :You're not on that channel!
 */
std::string TopicCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "TOPIC",
                                                    client.getNickName());

  std::string channelName = message.params[0];
  std::string replyStr = "", topic;

  Channel* channel = Channel::findChannel(channelName);
  if (channel == NULL)
    return ReplyUtility::makeErrNoSuchChannelReply(client, channelName);

  if (message.params.size() == 1) {
    topic = channel->getTopic();
    if (topic.empty())
      return ReplyUtility::makeNoTopicReply(client, channelName);

    replyStr += ReplyUtility::makeTopicReply(client, *channel);
    replyStr += ReplyUtility::makeCreationTimeReply(client, *channel);
  } else {
    for (size_t i = 1; i < message.params.size(); i++) {
      if (i != 1) topic += " ";
      topic += message.params[i];
    }
    if (channel->isUserInChannel(client.getNickName()) == false)
      return ReplyUtility::makeErrNotInChannelReply(client, channelName);

    if (channel->getChannelMode().find('t') != std::string::npos &&
        channel->isOperator(client) == false)
      return ReplyUtility::makeErrChanOPrivsNeededReply(client, channelName,
                                                        "topic");

    if (topic != channel->getTopic()) {
      std::vector<std::string> params;

      channel->setTopic(topic);
      params.push_back(channelName);
      params.push_back(channel->getTopic());
      channel->sendPart(
          client, ReplyUtility::makeCommandReply(client, "TOPIC", params));
    }
  }

  return replyStr;
}
