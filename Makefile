NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -I includes -MMD -MP

SRCS = src/main.c\
		src/parse.c\
		src/simu.c\
		src/routine.c\
		src/utils.c\
		src/dongle.c\
		src/time.c\
		src/monitor.c\
		src/scheduler.c\
		src/heap.c\
		src/queue.c\
		src/stop.c\
		src/request.c\
		src/mutex.c

OBJS = $(SRCS:.c=.o)
D_FILES = $(SRCS:.c=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS) $(D_FILES)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

-include $(D_FILES)