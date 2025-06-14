# ft_irc

ft_irc is a lightweight IRC server implemented in C++98, using BSD `kqueue` for high-performance event handling. It supports the core IRC commands and manages multiple clients, channels, and message routing according to the IRC protocol.

## Table of Contents

- [Features](#features)
- [Prerequisites](#prerequisites)
- [Building](#building)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Modules Overview](#modules-overview)
- [Supported Commands](#supported-commands)
- [Signal Handling](#signal-handling)
- [Memory Leak Checking](#memory-leak-checking)

## Features

- Connection handling with non-blocking sockets and `kqueue` event loop
- Client authentication (`PASS`, `USER`, `NICK`)
- Channel management (`JOIN`, `PART`, `TOPIC`, `MODE`, `INVITE`, `KICK`)
- Private and channel messages (`PRIVMSG`)
- Server and client modes
- Ping/pong keep-alive
- WHO query for user listings

## Prerequisites

- C++17 compiler (e.g., `clang++`, `g++`)
- BSD platform (macOS or FreeBSD) for `kqueue` support
- POSIX sockets and standard libraries

## Building

```sh
$ make
```

This will compile the `ircserv` executable in the project root.

## Usage

````shn
$ ./ircserv <port> <password>
```n
- `<port>`: TCP port the server will listen on (0–65535)
- `<password>`: server password required by clients to register

Example:

```sh
$ ./ircserv 6667 secretpass
````

## Project Structure

```
├── Makefile             # build rules
├── README.md            # this file
├── leaksScript.sh       # memory-leak check helper
└── src
    ├── main.cpp         # program entry point and signal setup
    ├── server
    │   └── Server.cpp   # core server class and event loop
    ├── client
    │   └── Client.cpp   # client connection and state management
    ├── channel
    │   └── Channel.cpp  # IRC channel representation
    ├── command
    │   ├── CommandHandler.cpp  # dispatches parsed messages
    │   ├── MessageHandler.cpp  # parses raw IRC messages
    │   ├── PassCommand.cpp     # PASS command
    │   ├── UserCommand.cpp     # USER command
    │   ├── NickCommand.cpp     # NICK command
    │   ├── JoinCommand.cpp     # JOIN command
    │   ├── PartCommand.cpp     # PART command
    │   ├── TopicCommand.cpp    # TOPIC command
    │   ├── ModeCommand.cpp     # MODE command
    │   ├── InviteCommand.cpp   # INVITE command
    │   ├── KickCommand.cpp     # KICK command
    │   ├── PrivmsgCommand.cpp  # PRIVMSG command
    │   ├── PingCommand.cpp     # PING command
    │   ├── WhoCommand.cpp      # WHO command
    │   └── QuitCommand.cpp     # QUIT command
    ├── utils
    │   ├── NumericReply.cpp    # numeric reply definitions
    │   ├── ReplyUtility.cpp    # helper for formatting replies
    │   └── StringUtility.cpp   # string parsing utilities
    └── client
        └── Client.cpp         # (duplicate path listing client impl)
```

## Modules Overview

- **main.cpp**: Sets up signal handlers, initializes the singleton `Server`, and starts the event loop.
- **server/Server.cpp**: Implements the singleton `Server` class:
  - Initializes listening socket and `kqueue`
  - Accepts new connections and registers read/write events
  - Reads raw data, buffers it, and triggers write events when a full message is received
  - Dispatches parsed messages to `CommandHandler` and sends replies
- **client/Client.cpp**: Represents a connected client, tracks registration state, nickname, username, and associated socket.
- **channel/Channel.cpp**: Represents a channel, tracks members, topic, modes, and invitation lists.
- **command/CommandHandler.cpp** & **MessageHandler.cpp**: Central dispatch for parsed IRC messages to individual command implementations.
- **command/\*Command.cpp**: Implements each IRC command according to the IRC protocol semantics.
- **utils**: Helper classes for numeric replies, reply formatting, and string manipulation.

## Supported Commands

- PASS: set connection password
- USER: set username and real name
- NICK: set or change nickname
- JOIN: join or create a channel
- PART: leave a channel
- TOPIC: set or query channel topic
- MODE: set user or channel modes
- INVITE: invite a user to a channel
- KICK: remove a user from a channel
- PRIVMSG: send private or channel messages
- PING: keep-alive ping/pong
- WHO: list users matching a mask
- QUIT: disconnect from the server

## Signal Handling

- **SIGPIPE**: ignored to prevent crashes on broken pipes
- **SIGINT**, **SIGQUIT**: caught to trigger graceful shutdown of the server event loop

## Memory Leak Checking

A helper script `leaksScript.sh` is provided to check for leaks on macOS:

```sh
$ ./leaksScript.sh ./ircserv 6667 secretpass
```

---

_ft_irc_ follows the core RFC 1459 IRC protocol semantics and is intended as a learning project. Feel free to extend functionality or adapt it to other platforms.
