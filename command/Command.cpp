#include "Command.hpp"

std::pair<int, std::string> JoinCommand::execute(Client& client,
                                                 Message& message) {}

std::pair<int, std::string> PassCommand::execute(Client& client,
                                                 Message& message) {
  if (message.params.size() < 1) {
    return std::make_pair(
        ERR_NEEDMOREPARAMS,
        "* PASS " + mNumericReply.getReply(ERR_NEEDMOREPARAMS));
  }
  client.setPassword(message.params[0]);
  return std::make_pair(0, "");
}

std::pair<int, std::string> NickCommand::execute(Client& client,
                                                 Message& message) {
  if (message.params.size() < 1) {
    return std::make_pair(
        ERR_NEEDMOREPARAMS,
        "* NICK " + mNumericReply.getReply(ERR_NEEDMOREPARAMS));
  }

  if (!isValidNickName(message.params[0])) {
    return std::make_pair(ERR_ERRONEUSNICKNAME,
                          "* " + message.params[0] + " " +
                              mNumericReply.getReply(ERR_ERRONEUSNICKNAME));
    client.setNickName(message.params[0]);
  }

  if (client.getUserName() != "") {
    if (client.isPasswordValid(client.getPassword())) {
      return std::make_pair(
          RPL_WELCOME,
          "* " + message.params[0] + " " + mNumericReply.getReply(RPL_WELCOME));
    }
    return std::make_pair(-1,
                          "임시 ERROR :Closing link: (123@172.29.0.1) [Access "
                          "denied by configuration]");
  }
  return std::make_pair(0, "");
}

bool NickCommand::isValidNickName(const std::string& nickName) {
  if (nickName.length() < 1 || nickName.length() > 9) return false;

  if (!std::isalpha(nickName[0]) &&
      std::string("[\\]^_{|}").find(nickName[0]) == std::string::npos)
    return false;

  for (int i = 1; i < nickName.length(); i++) {
    if (!std::isalnum(nickName[i]) &&
        std::string("[\\]^_{|}").find(nickName[i]) == std::string::npos) {
      return false;
    }
  }

  return true;
}

std::pair<int, std::string> UserCommand::execute(Client& client,
                                                 Message& message) {
  if (message.params.size() < 4) {
    return std::make_pair(
        ERR_NEEDMOREPARAMS,
        "* USER " + mNumericReply.getReply(ERR_NEEDMOREPARAMS));
  }

  client.setUserName(message.params[0]);
  client.setRealName(message.params[3]);
  if (client.getNickName() != "") {
    if (client.isPasswordValid(client.getPassword())) {
      return std::make_pair(
          RPL_WELCOME,
          "* " + message.params[0] + " " + mNumericReply.getReply(RPL_WELCOME));
    }
    return std::make_pair(-1,
                          "임시 ERROR :Closing link: (123@172.29.0.1) [Access "
                          "denied by configuration]");
  }
  return std::make_pair(0, "");
}
