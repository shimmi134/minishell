# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shimi-be <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/11 11:53:14 by shimi-be          #+#    #+#              #
#    Updated: 2025/05/09 17:14:18 by shimi-be         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
SRC = test.c
LIB = libft.a
FLAGS = -Wall -Werror -Wextra

OBJ = $(SRC:.c=.o)

all : $(NAME)

$(NAME): $(OBJ) $(INCLUDE)
	cc  $(SRC) -L/usr/include -lreadline -lhistory $(LIB)  -g  -o $(NAME)

%.o: %.c $(INCLUDE) Makefile
	cc -L/usr/include -lreadline -lhistory $(LIB) -c $< -o $@

clean:
	rm -rf $(OBJ) $(OBJ_BONUS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY : all clean fclean re bonus
