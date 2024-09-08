#include "command/JoinCommand.hpp"

/**
 * :irc.local 461 test1 JOIN :Not enough parameters.
 */
std::string JoinCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "JOIN");

  if (client.getNickName() == "" || client.getUserName() == "")
    return ReplyUtility::makeErrNotRegisteredReply(client, "JOIN");

  if (StringUtility::isValidChannelName(message.params[0]))
    return ReplyUtility::makeErrInvalidChannelNameReply(client,
                                                        message.params[0]);

  std::string replyStr = "";
  while (message.params[0].size()) {
    size_t pos = message.params[0].find(",");
    if (pos == std::string::npos) pos = message.params[0].size();

    std::string channelName = message.params[0].substr(0, pos);
    message.params[0].erase(0, pos + (pos != message.params[0].size()));
    Channel* channel = Channel::findChannel(channelName);

    if (channel == NULL) channel = Channel::createChannel(client, channelName);

    replyStr += ":" + client.getNickName() + "!" + client.getUserName() + "@" +
                client.getHostName() + " JOIN :" + message.params[0] + "\r\n";
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
 */
