NAME        = ircserv
SRCS        = main.cpp \
							Server.cpp \
							Client.cpp \
							Channel.cpp \
							MessageHandler.cpp \
							command/Command.cpp \
							command/CommandHandler.cpp \
							command/NumericReply.cpp \
							command/ReplyUtility.cpp

OBJDIR      = build
OBJS        = $(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SRCS)))
DEPS        = $(patsubst %.cpp, $(OBJDIR)/%.d, $(notdir $(SRCS)))

VPATH			  = command

CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98 -MMD -MP

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
	cd docker && docker-compose up --no-log-prefix --build

down:
	cd docker && docker-compose down


.PHONY: all clean fclean re
