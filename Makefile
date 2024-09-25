NAME        = ircserv
SRCS        = main.cpp \
							Server.cpp \
							Client.cpp \
							Channel.cpp \
							MessageHandler.cpp \
							JoinCommand.cpp \
							NickCommand.cpp \
							PassCommand.cpp \
							UserCommand.cpp \
							ModeCommand.cpp \
							PrivmsgCommand.cpp \
							PartCommand.cpp \
							PingCommand.cpp \
							WhoCommand.cpp \
							InviteCommand.cpp \
							KickCommand.cpp \
							TopicCommand.cpp \
							QuitCommand.cpp \
							CommandHandler.cpp \
							NumericReply.cpp \
							ReplyUtility.cpp \
							StringUtility.cpp \


SRCDIR		= src
OBJDIR      = build

OBJS        = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))
DEPS        = $(patsubst %.cpp, $(OBJDIR)/%.d, $(SRCS))

VPATH       = $(SRCDIR):$(SRCDIR)/server:$(SRCDIR)/client:$(SRCDIR)/channel:$(SRCDIR)/command:$(SRCDIR)/utils

CXX         = c++
CXXFLAGS    = -Wall -Werror -Wextra -Iinclude -std=c++98 -MMD -MP -g 

INTERFACE	= en0
IRC_SERVER	= $(shell ifconfig $(INTERFACE) | grep 'inet ' | grep -v 'inet6' | awk '{print $$2}')
NICK		= test
PORT		= 6667

all:    $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

-include $(DEPS)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

re: 
	make fclean
	make all

up:
	cd docker && docker compose up --no-log-prefix --build 

down:
	cd docker && docker compose down 
	docker stop $$(docker ps -qa)
	docker rm $$(docker ps -qa)

irssi:
	@if [ -z "$(PASS)" ]; then \
		docker run -it irssi irssi -c $(IRC_SERVER) -n $(NICK) -p $(PORT); \
	else \
		docker run -it irssi irssi -c $(IRC_SERVER) -w $(PASS) -n $(NICK) -p $(PORT); \
	fi

.PHONY: all clean fclean re

