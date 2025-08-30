# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shimi-be <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/11 11:53:14 by shimi-be          #+#    #+#              #
#    Updated: 2025/08/26 19:17:25 by shimi-be         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
SRC = exec/exec_utils.c exec/exec_utils2.c exec/exec_utils3.c exec/exec_utils4.c		\
	  exec/exec_utils5.c exec/exec_utils6.c exec/exec_utils7.c exec/exec_utils8.c		\
	  exec/exec_utils9.c exec/exec_utils10.c exec/exec_utils11.c exec/exec_utils12.c	\
	  builtin/builtins_utils.c builtin/builtins_utils2.c builtin/builtins_utils3.c		\
	  builtin/builtins_utils4.c builtin/builtins_utils5.c builtin/builtins_utils6.c		\
	  cmd_struct/add_nested_utils.c cmd_struct/cmd_struct_init.c ft_utils/ft_split.c 	\
	  ft_utils/ft_strcmp.c ft_utils/ft_strjoin.c ft_utils/ft_strtrim.c heredoc/heredoc.c	\
	  heredoc/heredoc_utils.c heredoc/heredoc_utils2.c heredoc/heredoc_utils3.c quotes/lexer.c 		\
	  quotes/nested_utils.c quotes/nested_utils2.c quotes/nested_utils3.c quotes/quote_utils2.c 	\
	  quotes/quotes_utils.c quotes/quote_utils3.c quotes/quote_helpers.c tokens/new_token.c			\
	  tokens/handle_tokens_in_cmd_struct.c tokens/parsing.c tokens/token_type.c tokens/token_utils.c	\
	  utils/allocate_utils.c utils/cleanup.c utils/redirect_utils.c utils/var_expand.c utils/assign_utils.c \
	  utils/cmd_struct_utils.c utils/utils.c exec/exec_utils13.c builtin/builtins_utils7.c main.c

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
	@$(MAKE) -C libft
	@mv libft/libft.a .

clean:
	@rm -rf $(OBJ)
	@$(MAKE) -s -C libft clean
	@echo "removed .o files!"

fclean: clean
	@rm -f $(NAME)
	@rm -f $(LIB)
	@$(MAKE) -s -C libft fclean
	@echo "removed minishell executable!"

re: fclean all

.PHONY: all clean fclean re bonus
