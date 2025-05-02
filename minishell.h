#ifndef MINISHELL_H
#define MINISHELL_H

#include<stdio.h>
#include<stddef.h>
#include<stdlib.h>
#include <unistd.h>
#include <stdbool.h>

typedef enum
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
    TOKEN_SEPARATOR,
    TOKEN_QUOTE_SINGLE,
    TOKEN_QUOTE_DOUBLE,
    TOKEN_VARIABLE,
    TOKEN_INVALID
} token_type;

typedef struct s_token 
{
    token_type type;
    char *value;
    bool inside_quotes;
    struct s_token *next;
} t_token;

/*
typedef struct s_command
{

}
*/

char *ft_strdup (char *str);
char **ft_split(char *str);
char *ft_strdup_char (char c);
char *ft_strdup_char (char c);
char *add_quoted_word(char *str, int *i, int type);
token_type find_token_type(char *str);
t_token *new_token(token_type type, char *value, int flag);
t_token *lexer (char *str);
t_token *lexer (char *str);
t_token *add_word(char *str, int *i);
t_token	*handle_quote(char *str, int *i);
int has_token(char *str);
int skip (char *str, int i);
int is_word(char *str);
int is_token(char c);
void free_tokens(t_token *head);
void print_list(t_token *head);
void print_enum(t_token *list);
#endif
