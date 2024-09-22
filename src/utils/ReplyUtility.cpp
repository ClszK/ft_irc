#include "utils/ReplyUtility.hpp"

std::string ReplyUtility::makeSuccessConnectReply(Client& client) {
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
std::string ReplyUtility::makeWelcomeReply(Client& client) {
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
std::string ReplyUtility::makeYourHostReply(Client& client) {
  std::stringstream ss;
  std::string str = NumericReply::getReply(RPL_YOURHOST);
  const std::string& serverName = client.getServerName();
  const std::string& version = client.getVersion();

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
std::string ReplyUtility::makeCreatedReply(Client& client) {
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
std::string ReplyUtility::makeMyInfoReply(Client& client) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << std::setw(3)
     << std::setfill('0') << RPL_MYINFO << " " << client.getServerName() << " "
     << client.getVersion() << " " << client.getAvailableUserMode() << " "
     << client.getAvailableChannelMode() << "\r\n";
  return ss.str();
}

std::string ReplyUtility::makeServerSupportedReply(Client& client) {
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
 */
std::string ReplyUtility::makeTopicReply(Client& client, Channel& channel) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << RPL_TOPIC << " "
     << client.getNickName() << " " << channel.getChannelName() << " :"
     << channel.getTopic() << "\r\n";

  return ss.str();
}

/**
 :irc.local 353 test = #123 :@test
 */
std::string ReplyUtility::makeNamReply(Client& client, Channel& channel) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << RPL_NAMREPLY << " "
     << client.getNickName() << " = " << channel.getChannelName() << " :";

  const std::vector<Client*>& userList = channel.getUserList();
  const std::vector<Client*>& gmList = channel.getGMList();

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
std::string ReplyUtility::makeEndOfNamesReply(Client& client,
                                              Channel& channel) {
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
std::string ReplyUtility::makeChannelModeIsReply(Client& client,
                                                 Channel& channel) {
  std::stringstream ss;
  const std::string& channelMode = channel.getChannelMode();
  size_t kPos = channelMode.find('k');
  size_t lPos = channelMode.find('l');
  size_t paramCnt = (kPos != std::string::npos) + (lPos != std::string::npos);

  ss << ":" << client.getServerName() << " " << RPL_CHANNELMODEIS << " "
     << client.getNickName() << " " << channel.getChannelName();

  if (paramCnt == 2) {
    ss << " +" << channelMode << " " << channel.getChannelKey() << " :"
       << channel.getMaxUser() << "\r\n";
  } else if (paramCnt == 1) {
    ss << " +" << channelMode << " :";
    if (kPos != std::string::npos)
      ss << channel.getChannelKey() << "\r\n";
    else
      ss << channel.getMaxUser() << "\r\n";
  } else
    ss << " :+" << channelMode << "\r\n ";

  return ss.str();
}

std::string ReplyUtility::makeErrNeedMoreParamsReply(
    Client& client, const std::string& command) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << std::setw(3)
     << std::setfill('0') << ERR_NEEDMOREPARAMS << " * " << command << " "
     << NumericReply::getReply(ERR_NEEDMOREPARAMS) << "\r\n";
  return ss.str();
}

std::string ReplyUtility::makeErrNonicknameGivenReply(Client& client,
                                                      const std::string& str) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << std::setw(3)
     << std::setfill('0') << ERR_NONICKNAMEGIVEN << " * " << str << " "
     << NumericReply::getReply(ERR_NONICKNAMEGIVEN) << "\r\n";
  return ss.str();
}

std::string ReplyUtility::makeErrorReply(Client& client) {
  return "ERROR :Closing link: (" + client.getNickName() + "@" +
         client.getClientIp() + ") [Access denied by configuration]";
}

std::string ReplyUtility::makeErrNotRegisteredReply(
    Client& client, const std::string& command) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << std::setw(3)
     << std::setfill('0') << ERR_NOTREGISTERED << " * " << command << " "
     << NumericReply::getReply(ERR_NOTREGISTERED) << "\r\n";

  return ss.str();
}

std::string ReplyUtility::makeErrInvalidUserNameReply(
    Client& client, const std::string& command) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << std::setw(3)
     << std::setfill('0') << ERR_INVALIDUSERNAME << " " << client.getNickName()
     << " " << command << " " << NumericReply::getReply(ERR_INVALIDUSERNAME)
     << "\r\n";

  return ss.str();
}

std::string ReplyUtility::makeErrBadChannelKeyReply(
    Client& client, const std::string& channelName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_BADCHANNELKEY << " "
     << client.getNickName() << " " << channelName << " "
     << NumericReply::getReply(ERR_BADCHANNELKEY) << "\r\n";
  return ss.str();
}

std::string ReplyUtility::makeErrInvalidChannelNameReply(
    Client& client, const std::string& str) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_INVALIDCHANNELNAME << " "
     << client.getNickName() << " " << str << " "
     << NumericReply::getReply(ERR_INVALIDCHANNELNAME) << "\r\n";
  return ss.str();
}

std::string ReplyUtility::makeErrChannelIsFullReply(
    Client& client, const std::string& channelName) {
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
    Client& client, const std::string& channelName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_INVITEONLYCHAN << " "
     << client.getNickName() << " " << channelName << " "
     << NumericReply::getReply(ERR_INVITEONLYCHAN) << "\r\n";
  return ss.str();
}

std::string ReplyUtility::makeErrNoSuchChannelReply(
    Client& client, const std::string& channelName) {
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
std::string ReplyUtility::makeErrNotExistReply(Client& client,
                                               const std::string& channelName,
                                               char mode) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_NOTEXIST << " "
     << client.getNickName() << " " << channelName << " " << mode << " * "
     << NumericReply::getReply(ERR_NOTEXIST);

  if (mode == 'k')
    ss << "Syntax: <key>.\r\n";
  else if (mode == 'o')
    ss << "Syntax: <nick>.\r\n";
  else if (mode == 'l')
    ss << "Syntax: <limit>.\r\n";

  return ss.str();
}

/**
 * :irc.local 401 t 123 :No such nick
 */
std::string ReplyUtility::makeErrNoSuchNickReply(Client& client,
                                                 const std::string& nickName) {
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
std::string ReplyUtility::makePrivmsgReply(Client& client,
                                           const std::string& target,
                                           const std::string& message) {
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
    Client& client, const std::string& channelName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_CANNOTSENDTOCHAN << " "
     << client.getNickName() << " " << channelName << " "
     << NumericReply::getReply(ERR_CANNOTSENDTOCHAN) << "\r\n";

  return ss.str();
}

std::string ReplyUtility::makeErrUnknownModeReply(Client& client, char mode) {
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
    Client& client, const std::string& channelName) {
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
    Client& client, const std::string& channelName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_NOTINCHANNEL << " "
     << client.getNickName() << " " << channelName << " "
     << NumericReply::getReply(ERR_NOTINCHANNEL) << "\r\n";

  return ss.str();
}

std::string ReplyUtility::makeCommandReply(
    Client& client, const std::string& command,
    const std::vector<std::string>& params) {
  std::stringstream ss;

  ss << ":" << client.getNickName() << "!" << client.getUserName() << "@"
     << client.getClientIp() << " " << command << " ";

  for (size_t i = 0; i < params.size() - 1; i++) ss << params[i] << " ";

  ss << ":" << params[params.size() - 1] << "\r\n";

  return ss.str();
}

std::string ReplyUtility::makePartReply(Client& client,
                                        const std::string& channelName,
                                        const std::string& partMessage) {
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
    Client& client, const std::string& nickName) {
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
    Client& client, const std::string& nickName) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_ERRONEUSNICKNAME << " * "
     << nickName << " " << NumericReply::getReply(ERR_ERRONEUSNICKNAME)
     << "\r\n";

  return ss.str();
}

std::string ReplyUtility::makePongReply(Client& client,
                                        const std::string& message) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " PONG " << client.getServerName()
     << " :" << message << "\r\n";

  return ss.str();
}