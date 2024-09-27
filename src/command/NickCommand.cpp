#include "command/NickCommand.hpp"

std::string NickCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "NICK");

  if (message.params[0].empty())
    return ReplyUtility::makeErrNonicknameGivenReply(client);

  if (StringUtility::isValidNickName(message.params[0]) == false)
    return ReplyUtility::makeErrErroneusNickNameReply(client,
                                                      message.params[0]);

  if (client.getNickName() == message.params[0]) return "";

  if (Client::findClient(message.params[0]) != NULL)
    return ReplyUtility::makeErrAlreadyNickUseReply(client, message.params[0]);

  std::string replyStr = "";

  if (client.getRegistered()) {
    std::vector<std::string> params;

    params.push_back(message.params[0]);

    replyStr = ReplyUtility::makeCommandReply(client, "NICK", params);

    std::map<int, Client*> clients = Server::getInstance()->getClients();

    for (std::map<int, Client*>::iterator it = clients.begin();
         it != clients.end(); ++it) {
      Client* c = it->second;

      c->sendPrivmsg(replyStr);
    }

    client.setNickName(message.params[0]);
    return "";
  } else {
    client.setNickName(message.params[0]);
    if (client.getUserName() != "") {
      if (client.isPasswordValid()) {
        client.setRegistered(true);
        std::cout << "???" << std::endl;
        replyStr = ReplyUtility::makeSuccessConnectReply(client);
      } else {
        client.sendPrivmsg(ReplyUtility::makeErrorReply(
            client, "Access denied by configuration"));
        Client::deleteClient(client.getSockFd());
        return "";
      }
    }
  }

  return replyStr;
}
