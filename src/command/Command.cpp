#include "command/Command.hpp"

/**
 * :irc.local 461 test1 JOIN :Not enough parameters.
 */
std::string JoinCommand::execute(Client& client, Message& message) {
  (void)client;
  if (message.params.size() < 1)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "JOIN");

  if (client.getNickName() == "" || client.getUserName() == "")
    return ReplyUtility::makeErrNotRegisteredReply(client, "JOIN");

  if (StringUtility::isValidChannelName(message.params[0]))
    return ReplyUtility::makeErrInvalidChannelNameReply(client,
                                                        message.params[0]);

  while (message.params[0].find(',') != std::string::npos) {
    std::string channelName =
        message.params[0].substr(0, message.params[0].find(','));
    Channel* channel = Channel::findChannel(channelName);

    if (channel == NULL) {
      Channel::createChannel(client, channelName);
    }

    message.params[0] =
        message.params[0].substr(message.params[0].find(',') + 1);
  }
  return "";
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

std::string PassCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "PASS");

  client.setPassword(message.params[0]);
  return "";
}

std::string NickCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "NICK");

  if (!client.setNickName(message.params[0]))
    return ReplyUtility::makeErrNonicknameGivenReply(client, message.params[0]);

  if (client.getUserName() != "") {
    if (client.isPasswordValid())
      return ReplyUtility::makeSuccessConnectReply(client);

    return ReplyUtility::makeErrorReply(client);
  }
  return "";
}

#include <iostream>
std::string UserCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 4)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "USER");

  if (!client.setUserName(message.params[0]))
    return ReplyUtility::makeErrorReply(client);
  client.setRealName(message.params[3]);
  if (client.getNickName() != "") {
    if (client.isPasswordValid())
      return ReplyUtility::makeSuccessConnectReply(client);

    return ReplyUtility::makeErrorReply(client);
  }
  return "";
}
