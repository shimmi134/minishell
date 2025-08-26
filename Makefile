# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shimi-be <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/11 11:53:14 by shimi-be          #+#    #+#              #
#    Updated: 2025/08/26 19:04:59 by shimi-be         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
SRC = exec/exec_utils.c exec/exec_utils2.c exec/exec_utils3.c exec/exec_utils4.c  	\
	  exec/exec_utils5.c exec/exec_utils6.c exec/exec_utils7.c exec/exec_utils8.c	\
	  exec/exec_utils9.c exec/exec_utils10.c exec/exec_utils11.c exec/exec_utils12.c\
	  builtin/builtins_utils.c builtin/builtins_utils2.c builtin/builtins_utils3.c	\
	  builtin/builtins_utils4.c builtin/builtins_utils5.c builtin/builtins_utils6.c	\



main.c													

LIB = libft.a
FLAGS = -Wall -Werror -Wextra
HEADER = minishell.h
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIB) $(HEADER)
	@cc $(OBJ) -L/usr/include -lreadline -lhistory $(LIB) -g -o $(NAME)
	@echo "Minishell compiled!"

%.o: %.c $(HEADER) Makefile
	cc $(FLAGS) -g -c $< -o $@

$(LIB):
	$(MAKE) -C libft
	mv libft/libft.a .

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
