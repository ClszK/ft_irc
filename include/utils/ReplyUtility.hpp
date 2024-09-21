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
  /* privmsg reply */
  static std::string makePrivmsgReply(Client& client, const std::string& target,
                                      const std::string& message);
  static std::string makeErrCannotSendToChanReply(
      Client& client, const std::string& channelName);
};
