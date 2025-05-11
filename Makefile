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
