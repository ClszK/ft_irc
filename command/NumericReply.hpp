#pragma once

#include <map>
#include <string>

class NumericReply {
 private:
  std::map<int, std::string> mReplies;

 public:
  NumericReply();
  std::string getReply(int code);
  ~NumericReply();
}