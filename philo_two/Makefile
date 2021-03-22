# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maria <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/20 22:16:53 by cwindom           #+#    #+#              #
#    Updated: 2021/03/21 14:47:14 by maria            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_two
FLAGS = -Wall -Wextra -Werror
CC = clang
HEADER = ./philo_two.h
SRC = get_time.c init.c lifetime.c philo_two.c utils.c threads.c
OBJ = $(SRC:%.c=%.o)
all: $(NAME)
$(NAME): $(OBJ)
	$(CC) $(FLAGS) -I -pthread -o $(NAME) $(OBJ)
%.o : %.c
	$(CC) -c $< -o $@ $(FLAGS) -I.
clean:
	rm -rf $(OBJ)
fclean: clean
	rm -rf $(NAME)
re: fclean all
.PHONY: all clean fclean re
