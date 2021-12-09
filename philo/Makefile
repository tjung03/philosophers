# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tjung <tjung@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/08 14:28:17 by tjung             #+#    #+#              #
#    Updated: 2021/12/09 18:49:09 by tjung            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =		philo

CC = 		gcc

CFLAGS =	-Wall -Wextra -Werror

SRCS =		main.c parsing.c tools.c action_dining.c action_get_forks.c init.c print.c recall.c thread.c

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
