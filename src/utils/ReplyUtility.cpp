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
     << "!" << client.getUserName() << "@" << client.getHostName() << "\r\n";

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
 :irc.local 353 test = #123 :@test
 */
std::string makeNamReply(Client& client, Channel& channel) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << std::setw(3)
     << std::setfill('0') << RPL_NAMREPLY << " " << client.getNickName()
     << " = " << channel.getChannelName() << " :";

  std::vector<Client*>& userList = channel.getUserList();
  std::vector<Client*>& gmList = channel.getGMList();

  size_t i;
  for (i = 0; i < gmList.size(); i++)
    ss << "@" << gmList[i]->getNickName() << " ";

  for (i = 0; i < userList.size(); i++) ss << userList[i]->getNickName() << " ";

  ss << "\r\n";
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
         client.getHostName() + ") [Access denied by configuration]";
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

std::string ReplyUtility::makeErrInvalidChannelNameReply(
    Client& client, const std::string& str) {
  std::stringstream ss;

  ss << ":" << client.getServerName() << " " << ERR_INVALIDCHANNELNAME << " "
     << client.getNickName() << " " << str << " "
     << NumericReply::getReply(ERR_INVALIDCHANNELNAME) << "\r\n";
  return ss.str();
}
