#include "command/JoinCommand.hpp"

std::string JoinCommand::joinKeyMode(Client& client, Channel& channel,
                                     const std::string& channelName,
                                     Message& message) {
  if (message.params.size() < 2)
    return ReplyUtility::makeErrBadChannelKeyReply(client, channelName);

  std::string key = StringUtility::parseComma(message.params[1]);

  if (key != channel.getChannelKey())
    return ReplyUtility::makeErrBadChannelKeyReply(client, channelName);
  return "";
}

/**
 * :irc.local 461 test1 JOIN :Not enough parameters.
 */
std::string JoinCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "JOIN");

  if (client.getNickName() == "" || client.getUserName() == "")
    return ReplyUtility::makeErrNotRegisteredReply(client, "JOIN");

  std::string replyStr = "";
  while (message.params[0].size()) {
    std::string channelName = StringUtility::parseComma(message.params[0]);

    if (!StringUtility::isValidChannelName(channelName)) {
      replyStr +=
          ReplyUtility::makeErrInvalidChannelNameReply(client, channelName);
      continue;
    }

    if (client.getConnectedChannels().size() > CHANLIMIT) {
      replyStr += ReplyUtility::makeErrChannelLimitReply(client, channelName);
      continue;
    }
    Channel* channel = Channel::findChannel(channelName);

    if (channel == NULL)
      channel = Channel::createChannel(client, channelName);
    else {
      if (channel->isUserInChannel(client.getNickName())) continue;
      if (channel->getChannelMode().find('k') != std::string::npos) {
        std::string tempReply =
            joinKeyMode(client, *channel, channelName, message);
        replyStr += tempReply;
        if (tempReply != "") continue;
      }
      if (channel->getChannelMode().find('l') != std::string::npos) {
        if (channel->getUserList().size() >= channel->getMaxUser()) {
          replyStr +=
              ReplyUtility::makeErrChannelIsFullReply(client, channelName);
          continue;
        }
      }
      if (channel->getChannelMode().find('i') != std::string::npos) {
        if (std::find(channel->getInvitedList().begin(),
                      channel->getInvitedList().end(),
                      &client) == channel->getInvitedList().end()) {
          replyStr +=
              ReplyUtility::makeErrInviteOnlyChanReply(client, channelName);
          continue;
        }
        channel->setInvitedListSub(client);
      }

      client.setConnectedChannel(channel);
      channel->setUserListAdd(client);
    }
    std::vector<std::string> params;

    params.push_back(channelName);
    std::string successJoinReply =
        ReplyUtility::makeCommandReply(client, "JOIN", params);
    channel->sendPrivmsg(client, successJoinReply);
    replyStr += successJoinReply;
    if (channel->getTopic() != "") {
      replyStr += ReplyUtility::makeTopicReply(client, *channel);
      replyStr += ReplyUtility::makeCreationTimeReply(client, *channel);
    }
    replyStr += ReplyUtility::makeNamReply(client, *channel);
    replyStr += ReplyUtility::makeEndOfNamesReply(client, *channel);
  }
  return replyStr;
}
/**
  :test!root@192.168.65.1 JOIN :#123
 :irc.local 353 test = #123 :@test
 :irc.local 366 test #123 :End of /NAMES list.

 :test_!root@192.168.65.1 JOIN :#123
:irc.local 353 test_ = #123 :@test test_
:irc.local 366 test_ #123 :End of /NAMES list.

 "( "=" / "*" / "@" ) <channel>
               :[ "@" / "+" ] <nick> *( " " [ "@" / "+" ] <nick> )

 RPL_NAMREPLY

  topic이 설정되어 있을 때 추가되는 메시지.
 :irc.local 332 b #123 :a
 :irc.local 333 b #123 a!1@192.168.65.1 :1727199546
 */
