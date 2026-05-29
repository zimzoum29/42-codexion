NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = main.c\
		parse.c\
		simu.c\
		routine.c\
		utils.c\
		dongle.c\
		time.c\
		monitor.c\
		scheduler.c\
		heap.c\
		queue.c\
		stop.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all