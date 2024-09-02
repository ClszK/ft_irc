#pragma once

#include <arpa/inet.h>

#include <cstring>

class SocketAddr {
 public:
  SocketAddr() { std::memset(&mAddr, 0, sizeof(mAddr)); }
  SocketAddr(int port) {
    std::memset(&mAddr, 0, sizeof(mAddr));
    mAddr.sin_family = AF_INET;
    mAddr.sin_port = htons(port);
    mAddr.sin_addr.s_addr = INADDR_ANY;
  }

  operator struct sockaddr *() { return (struct sockaddr*)&mAddr; }
  operator const struct sockaddr *() const { return (struct sockaddr*)&mAddr; }
  operator struct sockaddr_in *() { return &mAddr; }
  operator const struct sockaddr_in *() const { return &mAddr; }

 private:
  struct sockaddr_in mAddr;
};