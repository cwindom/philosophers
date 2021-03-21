# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maria <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/20 22:16:53 by cwindom           #+#    #+#              #
#    Updated: 2021/03/20 22:31:46 by maria            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_one
FLAGS = #-Wall -Wextra -Werror
CC = clang
HEADER = ./philo_one.h
SRC = get_time.c init.c lifetime.c philo_one.c utils.c threads.c
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
