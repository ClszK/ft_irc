#include "Client.hpp"

Client::Client() : mServerPassword(NULL) {}

Client::Client(const int sockFd, std::string* serverPassword)
    : mSockFd(sockFd), mServerPassword(serverPassword) {}

Client::~Client() {}
