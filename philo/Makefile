NAME =		philo

CC = 		gcc

CFLAGS =	-Wall -Wextra -Werror

SRCS =		main.c parsing.c tools.c ft_atoi.c \
			action.c died.c dining.c init.c monitor.c

OBJS =		$(SRCS:.c=.o)

.PHONY:		all clean fclean re

all:		$(NAME)

$(NAME):	$(OBJS)
			@echo "\n\033[0;33mCompiling..."
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lpthread
			@echo "\033[0m"

clean:
			@echo "\n\033[0;31mCleaning..."
			rm -rf $(OBJS)
			@echo "\033[0m"

fclean:		clean
			@echo "\033[0;31mRemoving executable..."
			rm -f $(NAME)
			@echo "\033[0m"

re: 		fclean all
