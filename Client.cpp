#include "Client.hpp"

Client::Client() : mServerPassword(NULL), mServer(NULL) {}

Client::Client(const int sockFd, std::string* serverPassword, Server* server)
    : mSockFd(sockFd), mServerPassword(serverPassword), mServer(server) {}

Client::~Client() {}
