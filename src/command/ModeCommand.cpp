#include "command/ModeCommand.hpp"

const std::set<char> ModeCommand::createAllowedModeSet() {
  std::set<char> tempSet;

  tempSet.insert('i');
  tempSet.insert('t');
  tempSet.insert('k');
  tempSet.insert('o');
  tempSet.insert('l');
  return tempSet;
}

const std::set<char> ModeCommand::allowedMode =
    ModeCommand::createAllowedModeSet();

/**
 * - i : 초대 전용 채널 설정 / 제거
 * - t : 채널 운영자에 대한 TOPIC 명령 제한 설정 / 제거
 * - k : 채널 키(비밀번호) 설정 / 제거
 * - o : 채널 운영자 권한 부여 / 받기
 * - l : 채널의 유저 제한을 설정 / 제거
 *
 * MODE &oulu +b *!*@*.edu +e *!*@*.bu.edu
 * MODE #bu +be *!*@*.edu *!*@*.bu.edu
 */
std::string ModeCommand::execute(Client& client, Message& message) {
  if (message.params.size() < 1)
    return ReplyUtility::makeErrNeedMoreParamsReply(client, "MODE");

  std::string channelName = message.params[0];

  Channel* channel = Channel::findChannel(channelName);
  if (channel == NULL)
    return ReplyUtility::makeErrNoSuchChannelReply(client, channelName);
  if (message.params.size() == 1)
    return ReplyUtility::makeChannelModeIsReply(client, *channel);

  // size_t paramIdx = 2, addMode;

  // for (size_t i = 0; i < channelSetMode.size(); i++) {
  //   if (channelSetMode[i] == '+')
  //     addMode = 1;
  //   else if (channelSetMode[i] == '-')
  //     addMode = 0;

  //   char mode = channelSetMode[i];

  //   switch (mode) {
  //     case 'i':
  //     case 't':
  //       if (addMode)
  //         channel->setChannelModeAdd(mode);
  //       else
  //         channel->setChannelModeSub(mode);
  //       break;
  //     case 'k':
  //       if (addMode) {
  //         if (message.params.size() <= paramIdx)
  //           return ReplyUtility::makeErrNotExistReply(client, channelName,
  //                                                     mode);
  //         channel->setChannelKey(message.params[paramIdx++]);
  //         channel->setChannelModeAdd(mode);
  //       } else {
  //         channel->setChannelKey("");
  //         channel->setChannelModeSub(mode);
  //       }
  //       break;
  //     case 'o':
  //       if (message.params.size() <= paramIdx)
  //         return ReplyUtility::makeErrNotExistReply(client, channelName,
  //         mode);
  //       std::string nickName = message.params[paramIdx++];
  //       Client* target = Client::findClient(nickName);

  //       if (addMode) {
  //         if (target == NULL)
  //           return ReplyUtility::makeErrNoSuchNickReply(client, nickName);
  //         channel->setGMListAdd(*target);
  //       } else
  //         channel->setGMListSub(*target);

  //       break;
  //   }
  // }
  return "";
}