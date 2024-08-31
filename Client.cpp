#include "Client.hpp"

Client::Client(const int sockFd) : mSockFd(sockFd), mIsCertified(false) {}

Client::~Client() {}
