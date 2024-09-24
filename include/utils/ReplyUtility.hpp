#pragma once

#include <iomanip>
#include <sstream>
#include <string>

#include "channel/Channel.hpp"
#include "client/Client.hpp"
#include "utils/NumericReply.hpp"

class ReplyUtility {
 public:
  static std::string makeSuccessConnectReply(Client& client);
  static std::string makeWelcomeReply(Client& client);
  static std::string makeYourHostReply(Client& client);
  static std::string makeCreatedReply(Client& client);
  static std::string makeMyInfoReply(Client& client);
  static std::string makeServerSupportedReply(Client& client);
  static std::string makeTopicReply(Client& client, Channel& channel);
  static std::string makeNamReply(Client& client, Channel& channel);
  static std::string makeEndOfNamesReply(Client& client, Channel& channel);
  static std::string makeChannelModeIsReply(Client& client, Channel& channel);
  static std::string makePrivmsgReply(Client& client, const std::string& target,
                                      const std::string& message);
  static std::string makeCommandReply(Client& client,
                                      const std::string& command,
                                      const std::vector<std::string>& params);
  static std::string makePongReply(Client& client, const std::string& message);
  static std::string makeWhoReply(Client& client, const Client* target,
                                  const std::string& channelName);
  static std::string makeWhoEndReply(Client& client,
                                     const std::string& channelName);
  static std::string makeChannelTimeStampReply(Client& client,
                                               Channel& channel);
  static std::string makePartReply(Client& client,
                                   const std::string& channelName,
                                   const std::string& partMessage);
  static std::string makeKickReply(Client& client,
                                   const std::string& channelName,
                                   const std::string& kickNick,
                                   const std::string& kickMessage);
  static std::string makeEndofInvitelstReply(Client& client,
                                             const std::string& command);
  static std::string makeInvitingReply(Client& client,
                                       const std::string& target,
                                       const std::string& channelName);
  static std::string makeNoTopicReply(Client& client,
                                      const std::string& channelName);

  static std::string makeErrNeedMoreParamsReply(Client& client,
                                                const std::string& command);
  static std::string makeErrNonicknameGivenReply(Client& client,
                                                 const std::string& str);
  static std::string makeErrorReply(Client& client);
  static std::string makeErrNotRegisteredReply(Client& client,
                                               const std::string& command);
  static std::string makeErrBadChannelKeyReply(Client& client,
                                               const std::string& channelName);
  static std::string makeErrInvalidUserNameReply(Client& client,
                                                 const std::string& command);
  static std::string makeErrInvalidChannelNameReply(Client& client,
                                                    const std::string& str);
  static std::string makeErrChannelIsFullReply(Client& client,
                                               const std::string& channelName);
  static std::string makeErrInviteOnlyChanReply(Client& client,
                                                const std::string& channelName);
  static std::string makeErrNoSuchChannelReply(Client& client,
                                               const std::string& channelName);
  static std::string makeErrNotExistReply(Client& client,
                                          const std::string& channelName,
                                          char mode);
  static std::string makeErrNoSuchNickReply(Client& client,
                                            const std::string& nickName);
  static std::string makeErrCannotSendToChanReply(
      Client& client, const std::string& channelName);
  static std::string makeErrNotInChannelReply(Client& client,
                                              const std::string& channelName);
  static std::string makeErrUnknownModeReply(Client& client, char mode);
  static std::string makeErrKeyAlreadySetReply(Client& client,
                                               const std::string& channelName);
  static std::string makeErrAlreadyNickUseReply(Client& client,
                                                const std::string& nickName);
  static std::string makeErrErroneusNickNameReply(Client& client,
                                                  const std::string& nickName);
  static std::string makeErrUnknownCommandReply(Client& client,
                                                const std::string& command);
  static std::string makeErrChanOPrivsNeededReply(
      Client& client, const std::string& channelName, char mode);
  static std::string makeErrChanOPrivsNeededReply(
      Client& client, const std::string& channelName,
      const std::string& command);
  static std::string makeErrUserNotInChannelReply(
      Client& client, const std::string& nickName,
      const std::string& channelName);
  static std::string makeErrUserNotOnChannelReply(
      Client& client, const std::string& channelName);
  static std::string makeErrUserOnChannelReply(Client& client,
                                               const std::string& nickName,
                                               const std::string& channelName);
  static std::string makeErrInviteOPrivsNeededReply(
      Client& client, const std::string& channelName);
};
