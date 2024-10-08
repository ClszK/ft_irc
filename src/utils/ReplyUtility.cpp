#include "utils/ReplyUtility.hpp"

std::string ReplyUtility::makeSuccessConnectReply(Client &client) {
  std::string result;

  result += makeWelcomeReply(client);
  // result += makeYourHostReply(client);
  // result += makeCreatedReply(client);
  // result += makeMyInfoReply(client);
  // result += makeServerSupportedReply(client);
  return result;
}

/**
 * Welcome to the Internet Relay Network
               <nick>!<user>@<host>"
*/
std::string ReplyUtility::makeWelcomeReply(Client &client) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << std::setw(3)
     << std::setfill('0') << RPL_WELCOME << " " << client.getNickName() << " "
     << NumericReply::getReply(RPL_WELCOME) << " " << client.getNickName()
     << "!" << client.getUserName() << "@" << client.getClientIp() << "\r\n";

  return ss.str();
}

/**
 * "Your host is <servername>, running version <ver>"
 */
std::string ReplyUtility::makeYourHostReply(Client &client) {
  std::stringstream ss;
  std::string str = NumericReply::getReply(RPL_YOURHOST);
  const std::string &serverName = client.getServerName();
  const std::string &version = client.getVersion();

  str.replace(str.find("<servername>"), serverName.size(), serverName);
  str.replace(str.find("<ver>"), version.size(), version);

  ss << ":" << client.getServerName() << " " << std::setw(3)
     << std::setfill('0') << RPL_YOURHOST << " " << client.getNickName() << " "
     << str << "\r\n";

  return ss.str();
}

/**
 * "This server was created <date>"
 */
std::string ReplyUtility::makeCreatedReply(Client &client) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << std::setw(3)
     << std::setfill('0') << RPL_CREATED << " " << client.getNickName() << " "
     << NumericReply::getReply(RPL_CREATED) << " " << client.getCreatedTime()
     << "\r\n";

  return ss.str();
}

/**
 * <servername> <version> <available user modes>
               <available channel modes>
*/
std::string ReplyUtility::makeMyInfoReply(Client &client) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << std::setw(3)
     << std::setfill('0') << RPL_MYINFO << " " << client.getServerName() << " "
     << client.getVersion() << " " << client.getAvailableUserMode() << " "
     << client.getAvailableChannelMode() << "\r\n";
  return ss.str();
}

std::string ReplyUtility::makeServerSupportedReply(Client &client) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << std::setw(3)
     << std::setfill('0') << RPL_SUPPORT << " "
     << "CASEMAPPING=" << CASEMAPPING << " "
     << "CHANLIMIT=" << CHANLIMIT << " "
     << "CHANMODES=" << CHANMODES << " "
     << "CHANNELLEN=" << CHANNELLEN << " "
     << "CHANTYPES=" << CHANTYPES << " "
     << "KEYLEN=" << KEYLEN << " "
     << "KICKLEN=" << KICKLEN << " "
     << "LINELEN=" << LINELEN << " "
     << "MAXTARGETS=" << MAXTARGETS << " "
     << "MODES=" << MODES << " "
     << "NAMELEN=" << NAMELEN << " "
     << "NETWORK=" << NETWORK << " "
     << "NICKLEN=" << NICKLEN << " "
     << "PREFIX=" << PREFIX << " "
     << "TOPICLEN=" << TOPICLEN << " "
     << "USERLEN=" << USERLEN << " "
     << "USERMODES=" << USERMODES << " " << NumericReply::getReply(RPL_SUPPORT)
     << "\r\n";

  return ss.str();
}

/**
 * "<channel> :<topic>"
 * :irc.local 332 a #123 :12
 */
std::string ReplyUtility::makeTopicReply(Client &client, Channel &channel) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << RPL_TOPIC << " "
     << client.getNickName() << " " << channel.getChannelName() << " :"
     << channel.getTopic() << "\r\n";

  return ss.str();
}

/**
 :irc.local 353 test = #123 :@test
 */
std::string ReplyUtility::makeNamReply(Client &client, Channel &channel) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << RPL_NAMREPLY << " "
     << client.getNickName() << " = " << channel.getChannelName() << " :";

  const std::vector<Client *> &userList = channel.getUserList();
  const std::vector<Client *> &gmList = channel.getGMList();

  size_t i;
  for (i = 0; i < gmList.size(); i++)
    ss << "@" << gmList[i]->getNickName() << " ";

  for (i = 0; i < userList.size(); i++) {
    if (std::find(gmList.begin(), gmList.end(), userList[i]) == gmList.end())
      ss << userList[i]->getNickName() << " ";
  }

  ss << "\r\n";
  return ss.str();
}

/**
 :irc.local 366 test #123 :End of /NAMES list.
 */
std::string ReplyUtility::makeEndOfNamesReply(Client &client,
                                              Channel &channel) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << RPL_ENDOFNAMES << " "
     << client.getNickName() << " " << channel.getChannelName() << " "
     << NumericReply::getReply(RPL_ENDOFNAMES) << "\r\n";

  return ss.str();
}

/**
 * :irc.local 324 t #123 +iklst test :10
 * :irc.local 324 t #123 :+t
 */
// std::string ReplyUtility::makeChannelModeIsReply(Client &client,
//                                                  Channel &channel) {
//   std::stringstream ss;
//   const std::string &channelMode = channel.getChannelMode();
//   size_t kPos = channelMode.find('k');
//   size_t lPos = channelMode.find('l');
//   size_t paramCnt = (kPos != std::string::npos) + (lPos !=
//   std::string::npos);

//   ss << ":" << client.getServerName() << " " << RPL_CHANNELMODEIS << " "
//      << client.getNickName() << " " << channel.getChannelName();

//   std::vector<std::string> params;

//   if (paramCnt == 2) {
//     ss << " +" << channelMode << " " << channel.getChannelKey() << " :"
//        << channel.getMaxUser() << "\r\n";
//   } else if (paramCnt == 1) {
//     ss << " +" << channelMode << " :";
//     if (kPos != std::string::npos)
//       ss << channel.getChannelKey() << "\r\n";
//     else
//       ss << channel.getMaxUser() << "\r\n";
//   } else
//     ss << " :+" << channelMode << "\r\n";

//   return ss.str();
// }
std::string ReplyUtility::makeChannelModeIsReply(Client &client,
                                                 Channel &channel) {
  std::stringstream ss;
  const std::string &channelMode = channel.getChannelMode();
  std::vector<std::string> params;

  ss << ":" << client.getServerName() << " " << RPL_CHANNELMODEIS << " "
     << client.getNickName() << " " << channel.getChannelName();

  params.push_back("+" + channelMode);
  for (std::string::const_iterator it = channelMode.begin();
       it != channelMode.end(); ++it) {
    if (*it == 'k')
      params.push_back(channel.getChannelKey());
    else if (*it == 'l')
      params.push_back(StringUtility::numberToString(channel.getMaxUser()));
  }

  for (std::vector<std::string>::iterator it = params.begin();
       it != params.end(); ++it) {
    if (it == params.end() - 1)
      ss << " :" << *it << "\r\n";
    else
      ss << " " << *it;
  }

  return ss.str();
}

/**
 * :irc.local 333 a #123 t5!root@192.168.65.1 :1727197400
 */
std::string ReplyUtility::makeCreationTimeReply(Client &client,
                                                Channel &channel) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << RPL_CREATIONTIME << " "
     << client.getNickName() << " " << channel.getChannelName() << " "
     << client.getNickName() << "!" << client.getUserName() << "@"
     << client.getClientIp() << " :" << channel.getTimeStamp() << "\r\n";

  return ss.str();
}

/**
 * :ft_ircd 461 * JOIN :Not enough parameters
 */
std::string ReplyUtility::makeErrNeedMoreParamsReply(
    Client &client, const std::string &command) {
  std::stringstream ss;
  std::string nickName = client.getNickName();

  if (nickName == "") nickName = "*";
  ss << ":" << client.getServerName() << " " << ERR_NEEDMOREPARAMS << " "
     << nickName << " " << command << " "
     << NumericReply::getReply(ERR_NEEDMOREPARAMS) << "\r\n";
  return ss.str();
}

std::string ReplyUtility::makeErrNonicknameGivenReply(Client &client) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << std::setw(3)
     << std::setfill('0') << ERR_NONICKNAMEGIVEN << " * "
     << NumericReply::getReply(ERR_NONICKNAMEGIVEN) << "\r\n";
  return ss.str();
}

std::string ReplyUtility::makeErrorReply(Client &client,
                                         const std::string &str) {
  std::string userName = client.getUserName();

  if (userName == "") userName = "811AAAAAB";

  return "ERROR :Closing link: (" + userName + "@" + client.getClientIp() +
         ") [" + str + "]\r\n";
}

std::string ReplyUtility::makeErrNotRegisteredReply(
    Client &client, const std::string &command) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << std::setw(3)
     << std::setfill('0') << ERR_NOTREGISTERED << " * " << command << " "
     << NumericReply::getReply(ERR_NOTREGISTERED) << "\r\n";

  return ss.str();
}

std::string ReplyUtility::makeErrInvalidUserNameReply(
    Client &client, const std::string &command) {
  std::stringstream ss;
  std::string nickName = client.getNickName();

  if (nickName == "") nickName = "*";

  ss << ":" << client.getServerName() << " " << ERR_INVALIDUSERNAME << " "
     << nickName << " " << command << " "
     << NumericReply::getReply(ERR_INVALIDUSERNAME) << "\r\n";

  return ss.str();
}

std::string ReplyUtility::makeErrBadChannelKeyReply(
    Client &client, const std::string &channelName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_BADCHANNELKEY << " "
     << client.getNickName() << " " << channelName << " "
     << NumericReply::getReply(ERR_BADCHANNELKEY) << "\r\n";
  return ss.str();
}

std::string ReplyUtility::makeErrInvalidChannelNameReply(
    Client &client, const std::string &str) {
  std::stringstream ss;
  ss << ":" << client.getServerName() << " " << ERR_INVALIDCHANNELNAME << " "
     << client.getNickName() << " " << str << " "
     << NumericReply::getReply(ERR_INVALIDCHANNELNAME) << "\r\n";
  return ss.str();
}

std::string ReplyUtility::makeErrChannelIsFullReply(
    Client &client, const std::string &channelName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_CHANNELISFULL << " "
     << client.getNickName() << " " << channelName << " "
     << NumericReply::getReply(ERR_CHANNELISFULL) << "\r\n";
  return ss.str();
}

/**
 * :server_name 473 <nickname> <channel> :Cannot join channel (+i)
 */
std::string ReplyUtility::makeErrInviteOnlyChanReply(
    Client &client, const std::string &channelName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_INVITEONLYCHAN << " "
     << client.getNickName() << " " << channelName << " "
     << NumericReply::getReply(ERR_INVITEONLYCHAN) << "\r\n";
  return ss.str();
}

std::string ReplyUtility::makeErrNoSuchChannelReply(
    Client &client, const std::string &channelName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_NOSUCHCHANNEL << " "
     << client.getNickName() << " " << channelName << " "
     << NumericReply::getReply(ERR_NOSUCHCHANNEL) << "\r\n";
  return ss.str();
}

/**
 * :irc.local 696 test #test k * :You must specify a parameter for the key mode.
 * Syntax: <key>.
 */
std::string ReplyUtility::makeErrNotExistReply(Client &client,
                                               const std::string &channelName,
                                               char mode) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_NOTEXIST << " "
     << client.getNickName() << " " << channelName << " " << mode << " * "
     << NumericReply::getReply(ERR_NOTEXIST);

  if (mode == 'k')
    ss << "key mode. Syntax: <key>.\r\n";
  else if (mode == 'o')
    ss << "op mode. Syntax: <nick>.\r\n";
  else if (mode == 'l')
    ss << "limit mode. Syntax: <limit>.\r\n";

  return ss.str();
}

/**
 * :irc.local 401 t 123 :No such nick
 */
std::string ReplyUtility::makeErrNoSuchNickReply(Client &client,
                                                 const std::string &nickName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_NOSUCHNICK << " "
     << client.getNickName() << " " << nickName << " "
     << NumericReply::getReply(ERR_NOSUCHNICK) << "\r\n";

  return ss.str();
}

/****  privmsg reply ****/
/* :gi!123@192.168.65.1 PRIVMSG #1 :hi
 * :test!user@192.168.65.1 PRIVMSG gi :hi
 */
std::string ReplyUtility::makePrivmsgReply(Client &client,
                                           const std::string &target,
                                           const std::string &message) {
  std::stringstream ss;

  ss << ":" << client.getNickName() << "!" << client.getUserName() << "@"
     << client.getClientIp() << " PRIVMSG " << target << " :" << message
     << "\r\n";

  return ss.str();
}

/*
172.018.000.002.06667-192.168.065.001.33059: :irc.local 404 a #123 :You cannot
send external messages to this channel whilst the +n (noextmsg) mode is set.
*/
std::string ReplyUtility::makeErrCannotSendToChanReply(
    Client &client, const std::string &channelName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_CANNOTSENDTOCHAN << " "
     << client.getNickName() << " " << channelName << " "
     << NumericReply::getReply(ERR_CANNOTSENDTOCHAN) << "\r\n";

  return ss.str();
}

std::string ReplyUtility::makeErrUnknownModeReply(Client &client, char mode) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_UNKNOWNMODE << " "
     << client.getNickName() << " " << mode << " "
     << NumericReply::getReply(ERR_UNKNOWNMODE) << "\r\n";

  return ss.str();
}

/**
 * :irc.local 467 gi #1234 :Channel key already set
 */

std::string ReplyUtility::makeErrKeyAlreadySetReply(
    Client &client, const std::string &channelName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_KEYSET << " "
     << client.getNickName() << " " << channelName << " "
     << NumericReply::getReply(ERR_KEYSET) << "\r\n";

  return ss.str();
}
/* part message */
/*
 * :irc.local 442 hi #123 :You're not on that channel
 */
std::string ReplyUtility::makeErrNotInChannelReply(
    Client &client, const std::string &channelName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_NOTINCHANNEL << " "
     << client.getNickName() << " " << channelName << " "
     << NumericReply::getReply(ERR_NOTINCHANNEL) << "\r\n";

  return ss.str();
}

std::string ReplyUtility::makeCommandReply(
    Client &client, const std::string &command,
    const std::vector<std::string> &params) {
  std::stringstream ss;

  ss << ":" << client.getNickName() << "!" << client.getUserName() << "@"
     << client.getClientIp() << " " << command << " ";

  for (size_t i = 0; i < params.size() - 1; i++) ss << params[i] << " ";

  ss << ":" << params[params.size() - 1] << "\r\n";

  return ss.str();
}

std::string ReplyUtility::makePartReply(Client &client,
                                        const std::string &channelName,
                                        const std::string &partMessage) {
  std::stringstream ss;

  if (partMessage == "") {
    ss << ":" << client.getNickName() << "!" << client.getUserName() << "@"
       << client.getClientIp() << " PART :" << channelName << "\r\n";
  } else {
    ss << ":" << client.getNickName() << "!" << client.getUserName() << "@"
       << client.getClientIp() << " PART " << channelName << " :" << partMessage
       << "\r\n";
  }

  return ss.str();
}

// test!user@192.168.65.1 PART :#123
// test!user@192.168.65.1 PART #123 :byebye

/**
 * :irc.local 433 test t5 :Nickname is already in use.
 */
std::string ReplyUtility::makeErrAlreadyNickUseReply(
    Client &client, const std::string &nickName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_NICKNAMEINUSE << " "
     << (client.getNickName() == "" ? "*" : client.getNickName()) << " "
     << nickName << " " << NumericReply::getReply(ERR_NICKNAMEINUSE) << "\r\n";

  return ss.str();
}

/**
 * :irc.local 432 * 123 :Erroneous Nickname
 */
std::string ReplyUtility::makeErrErroneusNickNameReply(
    Client &client, const std::string &nickName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_ERRONEUSNICKNAME << " * "
     << nickName << " " << NumericReply::getReply(ERR_ERRONEUSNICKNAME)
     << "\r\n";

  return ss.str();
}

std::string ReplyUtility::makePongReply(Client &client,
                                        const std::string &message) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " PONG " << client.getServerName()
     << " :" << message << "\r\n";

  return ss.str();
}

std::string ReplyUtility::makeErrUnknownCommandReply(
    Client &client, const std::string &command) {
  std::stringstream ss;
  std::string nickName = client.getNickName();

  if (nickName == "") nickName = "*";

  ss << ":" << client.getServerName() << " " << ERR_UNKNOWNCOMMAND << " "
     << nickName << " " << command << " "
     << NumericReply::getReply(ERR_UNKNOWNCOMMAND) << "\r\n";

  return ss.str();
}

/**
 * :irc.local 352 t5 #123 alice host1 irc.local alice H :0 Alice
 */
std::string ReplyUtility::makeWhoReply(Client &client, const Client *target,
                                       const std::string &channelName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << RPL_WHOREPLY << " "
     << client.getNickName() << " " << channelName << " "
     << target->getUserName() << " " << target->getClientIp() << " "
     << client.getServerName() << " " << target->getNickName() << " H :0 "
     << target->getRealName() << "\r\n";

  return ss.str();
}

/**
 * :irc.local 352 t5 #123 bob host2 irc.local bob H :0 Bob
 */

std::string ReplyUtility::makeWhoEndReply(Client &client,
                                          const std::string &channelName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << RPL_ENDOFWHO << " "
     << channelName << " " << NumericReply::getReply(RPL_ENDOFWHO) << "\r\n";

  return ss.str();
}

/**
 * :irc.local 329 t4 #test123 :1726976453
 */
std::string ReplyUtility::makeChannelTimeStampReply(Client &client,
                                                    Channel &channel) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << RPL_CHANNELTIMESTAMP << " "
     << client.getNickName() << " " << channel.getChannelName() << " :"
     << channel.getTimeStamp() << "\r\n";

  return ss.str();
}

/*:irc.local 482 a #123 :You must be a channel op or higher to kick a more
privileged user. kick일때 mode 일 때":You must be a channel op or higher to set
channel mode */
std::string ReplyUtility::makeErrChanOPrivsNeededReply(
    Client &client, const std::string &channelName, char mode) {
  static std::map<char, std::string> replyMap;
  if (replyMap.empty()) {
    replyMap['i'] = "i (inviteonly).";
    replyMap['t'] = "t (topiclock).";
    replyMap['k'] = "k (key).";
    replyMap['o'] = "o (op).";
    replyMap['l'] = "l (limit).";
    replyMap['n'] = "n (noextmsg).";
  }

  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_CHANOPRIVSNEEDED << " "
     << client.getNickName() << " " << channelName << " "
     << NumericReply::getReply(ERR_CHANOPRIVSNEEDED) << "set channel mode "
     << replyMap[mode] << "\r\n";

  return ss.str();
}

std::string ReplyUtility::makeErrChanOPrivsNeededReply(
    Client &client, const std::string &channelName,
    const std::string &command) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_CHANOPRIVSNEEDED << " "
     << client.getNickName() << " " << channelName << " "
     << NumericReply::getReply(ERR_CHANOPRIVSNEEDED);
  if (command == "kick")
    ss << "kick a more privileged user." << "\r\n";
  else if (command == "topic")
    ss << "change the topic." << "\r\n";

  return ss.str();
}

std::string ReplyUtility::makeKickReply(Client &client,
                                        const std::string &channelName,
                                        const std::string &kickNick,
                                        const std::string &kickMessage) {
  std::stringstream ss;

  if (kickMessage == "") {
    ss << ":" << client.getNickName() << "!" << client.getUserName() << "@"
       << client.getClientIp() << " KICK " << channelName << " " << kickNick
       << "\r\n";
  } else {
    ss << ":" << client.getNickName() << "!" << client.getUserName() << "@"
       << client.getClientIp() << " KICK " << channelName << " " << kickNick
       << " :" << kickMessage << "\r\n";
  }

  return ss.str();
}

/*
  : irc.local 441 test a #123 : They are not on that channel
*/
std::string ReplyUtility::makeErrUserNotInChannelReply(
    Client &client, const std::string &nickName,
    const std::string &channelName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_USERNOTINCHANNEL << " "
     << client.getNickName() << " " << nickName << " " << channelName << " "
     << NumericReply::getReply(ERR_USERNOTINCHANNEL) << "\r\n";

  return ss.str();
}

/*:irc.local 337 c :End of INVITE list*/
std::string ReplyUtility::makeEndofInvitelstReply(Client &client) {
  std::stringstream ss;
  std::string nickName = client.getNickName();

  if (nickName == "") nickName = "*";

  ss << ":" << client.getServerName() << " " << RPL_ENDOFINVITELIST << " "
     << nickName << " " << NumericReply::getReply(RPL_ENDOFINVITELIST)
     << "\r\n";

  return ss.str();
}

/*:irc.local 442 b #1 :You're not on that channel!*/
std::string ReplyUtility::makeErrUserNotOnChannelReply(
    Client &client, const std::string &channelName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_NOTINCHANNEL << " "
     << client.getNickName() << " " << channelName << " "
     << NumericReply::getReply(ERR_NOTINCHANNEL) << "\r\n";

  return ss.str();
}

/*:irc.local 443 c c #123 :is already on channel*/
std::string ReplyUtility::makeErrUserOnChannelReply(
    Client &client, const std::string &nickName,
    const std::string &channelName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_USERONCHANNEL << " "
     << client.getNickName() << " " << nickName << " " << channelName << " "
     << NumericReply::getReply(ERR_USERONCHANNEL) << "\r\n";

  return ss.str();
}

/*:irc.local 482 b #1 :You must be a channel op or higher to send an invite.*/
std::string ReplyUtility::makeErrInviteOPrivsNeededReply(
    Client &client, const std::string &channelName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_CHANOPRIVSNEEDED << " "
     << client.getNickName() << " " << channelName << " "
     << NumericReply::getReply(ERR_CHANOPRIVSNEEDED) << "send an invite.\r\n";

  return ss.str();
}

/**
 * :irc.local 341 a b :#12
 */
std::string ReplyUtility::makeInvitingReply(Client &client,
                                            const std::string &target,
                                            const std::string &channelName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << RPL_INVITING << " "
     << client.getNickName() << " " << target << " :" << channelName << "\r\n";

  return ss.str();
}

/**
 * :irc.local 331 a #123 :No topic is set
 */
std::string ReplyUtility::makeNoTopicReply(Client &client,
                                           const std::string &channelName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << RPL_NOTOPIC << " "
     << client.getNickName() << " " << channelName << " "
     << NumericReply::getReply(RPL_NOTOPIC) << "\r\n";

  return ss.str();
}

/**
 * :irc.local 462 * :You may not reregister
 */
std::string ReplyUtility::makeErrAlreadyRegistredReply(Client &client) {
  std::stringstream ss;
  std::string nickName = client.getNickName();

  if (nickName == "") nickName = "*";

  ss << ":" << client.getServerName() << " " << ERR_ALREADYREGISTRED << " "
     << nickName << " " << NumericReply::getReply(ERR_ALREADYREGISTRED)
     << "\r\n";

  return ss.str();
}

/**
 * : irc.local 405 test1 #29 :You are on too many channels
 */
std::string ReplyUtility::makeErrChannelLimitReply(
    Client &client, const std::string &channelName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_CHANNELLIMIT << " "
     << client.getNickName() << " " << channelName << " "
     << NumericReply::getReply(ERR_CHANNELLIMIT) << "\r\n";

  return ss.str();
}

std::string ReplyUtility::makeErrNoTextToSendReply(Client &client) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_NOTEXTTOSEND << " "
     << client.getNickName() << " " << NumericReply::getReply(ERR_NOTEXTTOSEND)
     << "\r\n";

  return ss.str();
}