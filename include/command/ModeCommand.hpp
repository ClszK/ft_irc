#pragma once

#include <set>

#include "command/ICommand.hpp"
#include "utils/StringUtility.hpp"

class ModeCommand : public ICommand {
 public:
  virtual std::string execute(Client& client, Message& message);

 private:
  static const std::set<char> allowedMode;

  static const std::set<char> createAllowedModeSet();
};