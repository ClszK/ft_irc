#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

int main() {
  int listen_fd, conn_fd, kq, nev, i;
  struct sockaddr_in server_addr;
  struct kevent change_event, events[10];
  char buffer[BUFFER_SIZE];

  // 소켓 생성
  listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  // 소켓을 non-blocking 모드로 설정
  if (fcntl(listen_fd, F_SETFL, O_NONBLOCK) == -1) {
    perror("fcntl");
    close(listen_fd);
    exit(EXIT_FAILURE);
  }

  // 서버 주소 설정
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(SERVER_PORT);

  // 소켓 바인딩
  if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    perror("bind");
    close(listen_fd);
    exit(EXIT_FAILURE);
  }

  // 소켓 리슨
  if (listen(listen_fd, 10) == -1) {
    perror("listen");
    close(listen_fd);
    exit(EXIT_FAILURE);
  }

  // kqueue 생성
  kq = kqueue();
  if (kq == -1) {
    perror("kqueue");
    close(listen_fd);
    exit(EXIT_FAILURE);
  }

  // 리슨 소켓을 kqueue에 등록
  EV_SET(&change_event, listen_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
  if (kevent(kq, &change_event, 1, NULL, 0, NULL) == -1) {
    perror("kevent");
    close(listen_fd);
    close(kq);
    exit(EXIT_FAILURE);
  }

  // 이벤트 루프
  while (1) {
    nev = kevent(kq, NULL, 0, events, 10, NULL);
    if (nev == -1) {
      perror("kevent");
      close(listen_fd);
      close(kq);
      exit(EXIT_FAILURE);
    }

    for (i = 0; i < nev; i++) {
      if (events[i].ident == listen_fd) {
        // 새로운 클라이언트 연결 처리
        while ((conn_fd = accept(listen_fd, NULL, NULL)) != -1) {
          printf("Accepted new connection: %d\n", conn_fd);

          // 새 클라이언트 소켓을 non-blocking 모드로 설정
          if (fcntl(conn_fd, F_SETFL, O_NONBLOCK) == -1) {
            perror("fcntl");
            close(conn_fd);
            continue;
          }

          // 새로운 연결의 읽기 이벤트를 kqueue에 등록
          EV_SET(&change_event, conn_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0,
                 NULL);
          if (kevent(kq, &change_event, 1, NULL, 0, NULL) == -1) {
            perror("kevent");
            close(conn_fd);
          }
        }

        if (errno != EAGAIN && errno != EWOULDBLOCK) {
          perror("accept");
        }
      } else if (events[i].filter == EVFILT_READ) {
        // 수신 버퍼에서 데이터 읽기
        int n = read(events[i].ident, buffer, sizeof(buffer) - 1);
        if (n > 0) {
          buffer[n] = '\0';
          printf("Received message: %s\n", buffer);

          // 클라이언트에 데이터 전송 (쓰기 이벤트 등록)
          EV_SET(&change_event, events[i].ident, EVFILT_WRITE,
                 EV_ADD | EV_ENABLE, 0, 0, NULL);
          if (kevent(kq, &change_event, 1, NULL, 0, NULL) == -1) {
            perror("kevent");
          }
        } else if (n == 0) {
          // 클라이언트 연결 종료
          printf("Connection closed: %d\n", events[i].ident);
          close(events[i].ident);
        } else {
          if (errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("read");
            close(events[i].ident);
          }
        }
      } else if (events[i].filter == EVFILT_WRITE) {
        // 송신 버퍼에 데이터 쓰기
        const char *msg = "Echo message\n";
        int n = write(events[i].ident, msg, strlen(msg));
        if (n == -1) {
          if (errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("write");
            close(events[i].ident);
          }
        } else {
          // 송신 완료 후 쓰기 이벤트 제거
          EV_SET(&change_event, events[i].ident, EVFILT_WRITE, EV_DELETE, 0, 0,
                 NULL);
          if (kevent(kq, &change_event, 1, NULL, 0, NULL) == -1) {
            perror("kevent");
            close(events[i].ident);
          }
        }
      }
    }
  }

  close(listen_fd);
  close(kq);
  return 0;
}
