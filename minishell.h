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
    bool new_word;
    struct s_token *next;
} t_token;


typedef struct s_command
{
    char *cmd;
    char **args;
    char *infile;
    char *outfile;
    int append;
    int heredoc;
    char *heredoc_delim;
    struct s_command *next;    
} t_cmd;

char *ft_strdup (char *str);
char **ft_split(char *str);
char *ft_strdup_char (char c);
char *ft_strdup_char (char c);
char *add_quoted_word(char *str, int *i, int type);
token_type find_token_type(char *str);
t_token *new_token(token_type type, char *value, int flag, int new_word);
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
int check_tokens (t_token *tokens);
void init_cmds(t_token *tokens);
void print_cmd_list(t_cmd *head);
#endif
