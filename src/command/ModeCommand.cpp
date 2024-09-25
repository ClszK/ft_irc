#include "command/ModeCommand.hpp"

/**
 * - i : 초대 전용 채널 설정 / 제거
 * - t : 채널 운영자에 대한 TOPIC 명령 제한 설정 / 제거
 * - k : 채널 키(비밀번호) 설정 / 제거
 * - o : 채널 운영자 권한 부여 / 받기
 * - l : 채널의 유저 제한을 설정 / 제거
 *
 * MODE &oulu +b *!*@*.edu +e *!*@*.bu.edu
 * MODE #bu +be *!*@*.edu *!*@*.bu.edu
 *
 *
 * t2!root@192.168.65.1 MODE #1234 +k :1234
 */
std::string ModeCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "MODE");

  std::string channelName = message.params[0];
  std::string replyStr = "";

  Channel* channel = Channel::findChannel(channelName);
  if (channel == NULL)
    return ReplyUtility::makeErrNoSuchChannelReply(client, channelName);
  if (message.params.size() == 1) {
    replyStr += ReplyUtility::makeChannelModeIsReply(client, *channel);
    replyStr += ReplyUtility::makeChannelTimeStampReply(client, *channel);
    return replyStr;
  }

  size_t paramIdx = 2, addMode = 1;
  std::string channelSetMode = message.params[1];
  std::string nickName = "", plusMode = "", removeMode = "";
  Client* target = NULL;
  std::vector<std::string> params;
  std::vector<Client*> GMList = channel->getGMList();
  int paramSize = message.params.size() - 2;

  for (size_t i = 0; i < channelSetMode.size(); i++) {
    if (channelSetMode[i] == '+') {
      addMode = 1;
      continue;
    } else if (channelSetMode[i] == '-') {
      addMode = 0;
      continue;
    }
    char mode = channelSetMode[i];

    if (channel->isOperator(client) == false) {
      if ((mode == 'o' || mode == 'k' || mode == 'l') && paramSize-- <= 0)
        replyStr +=
            ReplyUtility::makeErrNotExistReply(client, channelName, mode);
      else
        replyStr += ReplyUtility::makeErrChanOPrivsNeededReply(
            client, channelName, mode);
      continue;
    }

    if ((mode != 'o' || mode != 'l' || mode != 'k') &&
        ((addMode &&
          channel->getChannelMode().find(mode) != std::string::npos) ||
         (!addMode &&
          channel->getChannelMode().find(mode) == std::string::npos)))
      continue;
    switch (mode) {
      case 'n':
      case 'i':
      case 't':
        if (addMode) {
          channel->setChannelModeAdd(mode);
          plusMode += mode;
        } else {
          channel->setChannelModeSub(mode);
          removeMode += mode;
        }
        break;
      case 'k':
        if (addMode) {
          if (message.params.size() <= paramIdx) {
            replyStr +=
                ReplyUtility::makeErrNotExistReply(client, channelName, mode);
            break;
          }
          params.push_back(message.params[paramIdx]);
          channel->setChannelKey(message.params[paramIdx++]);
          channel->setChannelModeAdd(mode);
          plusMode += mode;
        } else {
          if (channel->getChannelKey() != message.params[paramIdx]) {
            replyStr +=
                ReplyUtility::makeErrKeyAlreadySetReply(client, channelName);
            break;
          }
          params.push_back(message.params[paramIdx]);
          channel->setChannelKey("");
          channel->setChannelModeSub(mode);
          removeMode += mode;
        }
        break;
      case 'o':
        if (message.params.size() <= paramIdx) {
          replyStr +=
              ReplyUtility::makeErrNotExistReply(client, channelName, mode);
          break;
        }
        nickName = message.params[paramIdx++];
        target = Client::findClient(nickName);
        if (target == NULL) {
          replyStr += ReplyUtility::makeErrNoSuchNickReply(client, nickName);
          break;
        }

        params.push_back(nickName);
        if (addMode) {
          if (channel->isOperator(*target)) break;
          channel->setGMListAdd(*target);
          plusMode += mode;
        } else {
          if (!channel->isOperator(*target)) break;
          channel->setGMListSub(*target);
          removeMode += mode;
        }
        break;
      case 'l':
        if (addMode) {
          if (message.params.size() <= paramIdx) {
            replyStr +=
                ReplyUtility::makeErrNotExistReply(client, channelName, mode);
            break;
          }
          params.push_back(message.params[paramIdx]);
          channel->setChannelModeAdd(mode);
          errno = 0;
          long limit =
              std::strtol(message.params[paramIdx++].c_str(), NULL, 10);
          if (errno == ERANGE || limit < 0) {
            replyStr +=
                ReplyUtility::makeErrNotExistReply(client, channelName, mode);
            continue;
          }
          channel->setMaxUser(limit);
          plusMode += mode;
        } else {
          channel->setChannelModeSub(mode);
          channel->setMaxUser(0);
          removeMode += mode;
        }
        break;
      default:
        replyStr += ReplyUtility::makeErrUnknownModeReply(client, mode);
    }
  }
  if (plusMode.size() > 0 || removeMode.size() > 0) {
    std::string responseMode;
    if (plusMode.size() > 0) responseMode += "+" + plusMode;
    if (removeMode.size() > 0) responseMode += "-" + removeMode;

    params.insert(params.begin(), responseMode);
    params.insert(params.begin(), channelName);
    std::string response =
        ReplyUtility::makeCommandReply(client, "MODE", params);
    std::cout << response << "!" << std::endl;
    channel->sendPart(client, response);
  }
  return replyStr;
}

/**
 * :t5!root@192.168.65.1 MODE #1234 :+i-nt
 * :t5!root@10.18.225.31 MODE #123 :+i-nt
 * :t4!root@192.168.65.1 MODE #test :+i-nt
 */
