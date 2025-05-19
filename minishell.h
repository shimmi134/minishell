#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

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
}						token_type;

typedef struct s_token
{
	token_type			type;
	char				*value;
	bool				inside_double;
	bool				inside_single;
	bool				new_word;
	struct s_token		*next;
}						t_token;

typedef struct s_command
{
	char				*cmd;
	char				**args;
	char				*infile;
	char				*outfile;
	int					append;
	int					heredoc;
	char				*heredoc_delim;
	struct s_command	*next;
}						t_cmd;

typedef struct s_shell
{
	char				*type;
	struct s_command	*command;
	struct s_shell		*next;
}						t_shell;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

char					*ft_strdup(char *str);
char					*ft_strdup_char(char c);
char					*add_quoted_word(char *str, int *i, int type);
char					*expand_var(t_token *token, t_env *env);
token_type				find_token_type(char *str);
t_token					*new_token(token_type type, char *value, int flag,
							int new_word);
t_token					*lexer(char *str);
t_token					*add_word(char *str, int *i);
t_token					*handle_quote(char *str, int *i, int type);
t_token					*assign_args(t_token *tokens, t_cmd *cmds);
t_token					*assign_ctl_tokens(t_token *token, t_cmd *cmd,
							t_env *envp);
t_cmd                   *new_cmd_token(t_token *tokens, t_env *envp);
int						has_token(char *str);
int						skip(char *str, int i);
int						is_word(char *str);
int						is_token(char c);
int						check_tokens(t_token *tokens);
int						arg_count(t_token *tokens);
void					free_tokens(t_token *head);
void					print_list(t_token *head);
void					print_enum(t_token *list);
void					print_cmd_list(t_cmd *head);
t_cmd					*init_cmds(t_token *tokens, t_env *envp);
char					**ft_split(char const *s, char c);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
size_t					ft_strlen(const char *s);
char					*ft_strjoin(char const *s1, char const *s2);
char					*ft_strtrim(char const *s1, char const *set);
t_env					*copy_env(char *envp[]);
t_env					*create_node(char *env);
char					*ft_strjoin(char const *s1, char const *s2);
#endif
