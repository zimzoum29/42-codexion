NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -I includes

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
		src/scheduler_utils.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all