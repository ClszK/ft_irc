# ft_irc

ft_irc는 C++98과 BSD `kqueue`를 사용해 구현한 경량 IRC 서버입니다. RFC 1459 기반 프로토콜을 준수하지만, 실제 상용 IRC 서버의 동작과 차이가 있어 InspIRCd 서버의 동작 방식을 모방하였으며, 테스트용 IRC 클라이언트로는 `irssi`를 사용했습니다.

추후 시연 영상을 추가할 예정입니다.

## 목차

- [주요 기능](#주요-기능)
- [호환성 및 테스트](#호환성-및-테스트)
- [필수 요구 사항](#필수-요구-사항)
- [빌드 및 실행](#빌드-및-실행)
- [프로젝트 구조](#프로젝트-구조)
- [모듈 개요](#모듈-개요)
- [지원 명령어](#지원-명령어)
- [메모리 누수 검사](#메모리-누수-검사)
- [시연 영상](#시연-영상)

## 주요 기능

- 논블로킹 소켓과 `kqueue` 기반 이벤트 루프를 통한 동시성 연결 처리
- 클라이언트 인증: `PASS`, `USER`, `NICK`
- 채널 관리: `JOIN`, `PART`, `TOPIC`, `MODE`, `INVITE`, `KICK`
- 개인 / 채널 메시지 전달: `PRIVMSG`
- 서버 및 클라이언트 모드 관리
- Ping/Pong을 이용한 연결 확인
- `WHO` 명령어로 사용자 목록 조회

## 호환성 및 테스트

- RFC 1459 공식 문서와 실제 상용 서버(InspIRCd)의 동작 차이를 고려하여 InspIRCd를 모델로 구현하였습니다.
- 호환성 테스트를 위해 IRC 클라이언트 `irssi`를 사용했습니다.

## 필수 요구 사항

- C++98 호환 컴파일러 (예: `clang++`, `g++`)
- macOS 또는 FreeBSD (kqueue 지원 환경)
- POSIX 소켓 API

## 빌드 및 실행

```sh
$ make
```

컴파일 후 루트 디렉터리에 `ircserv` 실행 파일이 생성됩니다.

```sh
$ ./ircserv <포트> <비밀번호>
```

- `<포트>`: 서버가 리스닝할 TCP 포트 (0~65535)
- `<비밀번호>`: 클라이언트 등록 시 필요

예시:

```sh
$ ./ircserv 6667 secretpass
```

## 프로젝트 구조

```
├── Makefile             # 빌드 규칙
├── README.md            # 프로젝트 소개 및 설명
├── leaksScript.sh       # 메모리 누수 검사 스크립트
└── src
    ├── main.cpp         # 진입점 및 시그널 설정
    ├── server
    │   └── Server.cpp   # 서버 클래스 및 kqueue 이벤트 루프
    ├── client
    │   └── Client.cpp   # 클라이언트 연결 및 상태 관리
    ├── channel
    │   └── Channel.cpp  # 채널 표현 및 멤버 관리
    ├── command
    │   ├── CommandHandler.cpp  # 명령어 디스패치
    │   ├── MessageHandler.cpp  # 메시지 파싱
    │   ├── PassCommand.cpp     # PASS
    │   ├── UserCommand.cpp     # USER
    │   ├── NickCommand.cpp     # NICK
    │   ├── JoinCommand.cpp     # JOIN
    │   ├── PartCommand.cpp     # PART
    │   ├── TopicCommand.cpp    # TOPIC
    │   ├── ModeCommand.cpp     # MODE
    │   ├── InviteCommand.cpp   # INVITE
    │   ├── KickCommand.cpp     # KICK
    │   ├── PrivmsgCommand.cpp  # PRIVMSG
    │   ├── PingCommand.cpp     # PING
    │   ├── WhoCommand.cpp      # WHO
    │   └── QuitCommand.cpp     # QUIT
    ├── utils
    │   ├── NumericReply.cpp    # 숫자 응답 정의
    │   ├── ReplyUtility.cpp    # 응답 포맷 유틸리티
    │   └── StringUtility.cpp   # 문자열 파싱 유틸리티
```

## 모듈 개요

- **main.cpp**: 시그널 핸들링 등록 후 `Server` 싱글톤 초기화 및 실행
- **server/Server.cpp**: 소켓 바인딩, kqueue 설정, 이벤트 수신 및 처리, `CommandHandler` 호출
- **client/Client.cpp**: 클라이언트 소켓, 닉네임/유저네임 상태 추적
- **channel/Channel.cpp**: 채널 멤버, 토픽, 모드, 초대 리스트 관리
- **command/MessageHandler.cpp**: 원시 IRC 메시지 파싱
- **command/CommandHandler.cpp**: 파싱된 메시지를 각 커맨드 구현체로 전달
- **command/\*Command.cpp**: IRC 프로토콜 각 명령어 로직 구현
- **utils**: 숫자 응답(Reply) 상수, 포맷 헬퍼, 문자열 처리 유틸리티

## 지원 명령어

- PASS, USER, NICK, JOIN, PART, TOPIC, MODE, INVITE, KICK, PRIVMSG, PING, WHO, QUIT

## 메모리 누수 검사

macOS 환경에서 다음 스크립트를 사용해 검사:

```sh
$ ./leaksScript.sh ./ircserv 6667 secretpass
```

## 시연 영상

_추후 기능 시연 영상을 이곳에 추가할 예정입니다._
