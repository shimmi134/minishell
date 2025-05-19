# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shimi-be <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/11 11:53:14 by shimi-be          #+#    #+#              #
#    Updated: 2025/05/19 15:49:53 by shimi-be         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
SRC = $(wildcard *.c) $(wildcard joe/*.c)
LIB = libft.a
FLAGS = -Wall -Werror -Wextra

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIB)
	cc $(OBJ) -L/usr/include -lreadline -lhistory $(LIB) -g -o $(NAME)

%.o: %.c $(INCLUDE) Makefile
	cc -g -c $< -o $@

libft.a:
	$(MAKE) -C libft
	mv libft/libft.a .

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
