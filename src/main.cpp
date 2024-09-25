#include "server/Server.hpp"

/**
 * signal
 * - 서버에서 시그널 발생
 * - 클라이언트에서 시그널 발생
 * fd 관리
 * channel 관리
 * client 관리
 * server 동적할당 관리
 */
int main(int argc, char* argv[]) {
  try {
    signal(SIGPIPE, SIG_IGN);  // ignore broken pipe
    signal(SIGINT, Server::signalHandler);
    signal(SIGQUIT, Server::signalHandler);

    Server* server = Server::getInstance(argc, argv);
    server->run();
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }

  return 0;
}
