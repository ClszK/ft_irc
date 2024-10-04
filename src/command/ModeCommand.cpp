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
 *
 */

std::string ModeCommand::execute(Client &client, Message &message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "MODE");
  if (!client.getRegistered())
    return ReplyUtility::makeErrNotRegisteredReply(client, "MODE");
  std::string channelName = message.params[0];
  std::string replyStr = "";
  Channel *channel = Channel::findChannel(channelName);
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
  Client *target = NULL;
  std::vector<std::string> params;
  std::vector<Client *> GMList = channel->getGMList();
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
      else if (mode == 'o' || mode == 'k' || mode == 'l' || mode == 't' ||
               mode == 'i' || mode == 'n')
        replyStr += ReplyUtility::makeErrChanOPrivsNeededReply(
            client, channelName, mode);
      else
        replyStr += ReplyUtility::makeErrUnknownModeReply(client, mode);
      continue;
    }

    if (!(mode == 'o' || mode == 'l' || mode == 'k') &&
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
        if (message.params.size() <= paramIdx) {
          replyStr +=
              ReplyUtility::makeErrNotExistReply(client, channelName, mode);
          break;
        }
        if (addMode) {
          if (channel->getChannelKey() != "") return "";
          params.push_back(message.params[paramIdx]);
          channel->setChannelKey(message.params[paramIdx++]);
          channel->setChannelModeAdd(mode);
          plusMode += mode;
        } else {
          if (channel->getChannelKey() == "") return "";
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
          if (channel->isOperator(*target) ||
              !channel->isUserInChannel(nickName))
            break;
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
          errno = 0;
          long limit =
              std::strtol(message.params[paramIdx++].c_str(), NULL, 10);
          if (errno == ERANGE || limit < 0) {
            replyStr +=
                ReplyUtility::makeErrNotExistReply(client, channelName, mode);
            errno = 0;
            continue;
          }
          params.push_back(StringUtility::numberToString(limit));
          channel->setChannelModeAdd(mode);
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
    channel->sendPart(response);
  }
  return replyStr;
}

/**
 * mode #channel +i channelMode
 * +l은 channelMode에 포함되어 있어도 되어야함
 *
 * 변경 사항 : 다중 모드 지원 x. 하나의 모드만 처리
 */

// std::string ModeCommand::modeParamOne(Client &client, Channel &channel) {
//   std::string replyStr = "";
//   replyStr += ReplyUtility::makeChannelModeIsReply(client, channel);
//   replyStr += ReplyUtility::makeChannelTimeStampReply(client, channel);
//   return replyStr;
// }

// std::string ModeCommand::execute(Client &client, Message &message) {
//   if (message.params.size() < 1)
//     return ReplyUtility::makeErrNeedMoreParamsReply(client, "MODE");

//   if (!client.getRegistered())
//     return ReplyUtility::makeErrNotRegisteredReply(client, "MODE");

//   std::string channelName = message.params[0];
//   std::string replyStr = "";

//   Channel *channel = Channel::findChannel(channelName);
//   if (channel == NULL)
//     return ReplyUtility::makeErrNoSuchChannelReply(client, channelName);
//   if (message.params.size() == 1) return modeParamOne(client, *channel);

//   std::string channelSetMode = message.params[1];
//   std::string executeMode = "";
//   size_t channelSetModeIdx = 0;
//   bool addMode = true, paramExist = (message.params.size() > 2);

//   if (channelSetMode[0] == '+') {
//     addMode = true;
//     channelSetModeIdx++;
//   } else if (channelSetMode[0] == '-') {
//     addMode = false;
//     channelSetModeIdx++;
//   }

//   char mode = channelSetMode[channelSetModeIdx];
//   if (channelSetModeIdx > channelSetMode.size()) return "";

//   if (channel->isOperator(client) == false) {
//     if ((mode == 'o' || mode == 'k' || mode == 'l') && paramExist)
//       return ReplyUtility::makeErrNotExistReply(client, channelName, mode);
//     else if (mode == 'o' || mode == 'k' || mode == 'l' || mode == 't' ||
//              mode == 'i' || mode == 'n')
//       return ReplyUtility::makeErrChanOPrivsNeededReply(client, channelName,
//                                                         mode);
//     else
//       return ReplyUtility::makeErrUnknownModeReply(client, mode);
//   }

//   const std::map<char, std::string> &channelModes =
//   channel->getChannelModes(); std::string nickName = ""; Client *target =
//   NULL;

//   if (!(mode == 'o' || mode == 'l') &&
//       ((addMode && channelModes.find(mode) != channelModes.end()) ||
//        (!addMode && channelModes.find(mode) == channelModes.end())))
//     return "";

//   if (mode == 'n' || mode == 'i' || mode == 't') {
//     if (addMode)
//       channel->setChannelModesAdd(mode, "");
//     else
//       channel->setChannelModesSub(mode);
//   } else if (mode == 'k' || mode == 'o' || mode == 'l') {
//     if (!paramExist || !(mode == 'l' && !addMode))
//       return ReplyUtility::makeErrNotExistReply(client, channelName, mode);

//     switch (mode) {
//       case 'k':
//         if (addMode)
//           channel->setChannelModesAdd(mode, message.params[2]);
//         else {
//           if (channel->getChannelKey() != message.params[2])
//             return ReplyUtility::makeErrKeyAlreadySetReply(client,
//             channelName);

//           channel->setChannelModesSub(mode);
//         }
//         break;
//       case 'o':
//         nickName = message.params[2];
//         if ((target = Client::findClient(nickName)) == NULL)
//           return ReplyUtility::makeErrNoSuchNickReply(client, nickName);

//         if (addMode) {
//           if (channel->isOperator(*target) ||
//               !channel->isUserInChannel(nickName))
//             return "";
//           channel->setGMListAdd(*target);
//           channel->setChannelModesAdd(mode, nickName);
//         } else {
//           if (!channel->isOperator(*target)) return "";
//           channel->setGMListSub(*target);
//           channel->setChannelModesSub(mode);
//         }
//         break;
//       case 'l':
//         if (addMode) {
//           errno = 0;
//           long limit = std::strtol(message.params[2].c_str(), NULL, 10);
//           if (errno == ERANGE || limit < 0) {
//             errno = 0;
//             return ReplyUtility::makeErrNotExistReply(client, channelName,
//                                                       mode);
//           }
//           channel->setChannelModesAdd(mode,
//                                       StringUtility::numberToString(limit));
//         } else
//           channel->setChannelModesSub(mode);
//         break;

//       default:
//         return ReplyUtility::makeErrUnknownModeReply(client, mode);
//     }
//   }

//   std::vector<std::string> params;
//   std::string responseMode = (addMode) ? "+" : "-";
//   responseMode += mode;
//   params.push_back(responseMode);
//   if (mode == 'o' || mode == 'k' || (mode == 'l' && addMode))
//     params.push_back(message.params[2]);

//   channel->sendPart(ReplyUtility::makeCommandReply(client, "MODE", params));

//   return "";
// }

/**
 * mode #channel +i param
 */

/**
 * :t5!root@192.168.65.1 MODE #1234 :+i-nt
 * :t5!root@10.18.225.31 MODE #123 :+i-nt
 * :t4!root@192.168.65.1 MODE #test :+i-nt
 */

/**
 *   switch (channelSetMode[channelSetModeIdx]) {
    case 'n':
    case 'i':
    case 't':
      if (addMode)
        channel->setChannelModesAdd(mode, "");
      else
        channel->setChannelModesSub(mode);
      break;
    case 'k':
      if (!paramExist)
        return ReplyUtility::makeErrNotExistReply(client, channelName, mode);

      if (addMode) {
        channel->setChannelModesAdd(mode, message.params[2]);
      } else {
        if (channel->getChannelKey() != message.params[2])
          return ReplyUtility::makeErrKeyAlreadySetReply(client, channelName);

        channel->setChannelModesSub(mode);
      }
      break;
    case 'o':
      if (!paramExist)
        return ReplyUtility::makeErrNotExistReply(client, channelName, mode);

      nickName = message.params[2];
      Client *target = Client::findClient(nickName);
      if (target == NULL)
        return ReplyUtility::makeErrNoSuchNickReply(client, nickName);

      if (addMode) {
        if (channel->isOperator(*target) || !channel->isUserInChannel(nickName))
          return "";
        channel->setGMListAdd(*target);
        channel->setChannelModesAdd(mode, nickName);
      } else {
        if (!channel->isOperator(*target)) return "";
        channel->setGMListSub(*target);
        channel->setChannelModesSub(mode);
      }
      break;

    default:
      break;
  }
 */