/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:05:53 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/21 14:56:58 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
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
	int					heredoc_fd;
	int					heredoc_quoted;
	int					exit_status;
	int					exit_status2;
	int					exit_code;
	struct s_command	*next;
}						t_cmd;

typedef struct s_shell
{
	char				*type;
	struct s_command	*command;
	int					*exit_status_code;
	struct s_shell		*next;
}						t_shell;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_heredoc
{
	char				*cmd;
	char				*heredoc_delim;
	char				**args;
	int					heredoc_fd;
	int					heredoc_quoted;
	struct s_heredoc	*next;
}						t_heredoc;

typedef struct s_quote_vars
{
	int					j;
	int					k;
	char				quote;
	char				*arr;
	int					new_word;
	int					first;
	int					type;
	int					quote_flag;
	struct s_token		**head;
}						t_quote_vars;

typedef struct s_struct_var
{
	t_token				**head;
	t_token				**current;
}						t_struct_var;

int						if_valid(char *str);
int						has_token(char *str);
int						skip(char *str, int i);
int						is_word(char *str);
int						is_token(char c);
int						check_tokens(t_token *tokens);
int						arg_count(t_token *tokens);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
int						ft_strcmp(char *s1, char *s2);
int						check_quotes(t_token *token, char *str);
int						read_heredoc(t_heredoc *hd_temp, t_env *env);
int						count_len(char **av);
int						count_commands(t_shell *sh);
int						ft_strspn(char *str, char *sep);
int						ft_atoi(const char *nptr);
int						ft_lensplit(char **split);
int						do_builtins(t_shell *elem, t_env **env);
int						corr_input(t_shell *elem);
int						do_export(t_shell *elem, t_env **env);
int						do_cd(t_shell *elem, t_env **env);
int						init_heredoc(t_heredoc *hd_temp, t_env *env,
							t_shell *element, int *fd_val);
int						do_echo(t_shell *elem, t_env **env);
int						do_pwd(void);
int						do_env(t_env **env);
int						do_unset(t_shell *elem, t_env **env);
int						ft_isalnum(int c);
int						ft_isdigit(int c);
int						env_len(t_env *env);
int						run_builtin(t_shell *elem, t_env **env);
int						export_error(char *arg);
int						assign_concat_flag(char *str, int i, t_token **current);
int						valiaa(char *str);
int						close_parent(int prev_fd, int has_next, int *next_pipe);
int						execute_loop_loop(t_shell *elem, t_env **env,
							int **last_status_ptr_out);
int	init_execute(t_token *node, t_token *head, t_env *env, int *exit_status);
int						pre_exec(char *line, t_env *env, int *exit_status);
int						check_line(char *str);
void					free_combined(int *exit_status, t_env *env);
void					do_heredoc(t_cmd *t_head, t_env *env, int *exit_status,
							int *fd_val);
void					execute_loop(t_shell *elem, t_env **env, int *fd_val,
							int **last_status_ptr_out);
void					do_struct(t_shell **element, t_cmd *command,
							int *exit_status);
void					child_outfile(t_shell *elem);
void					child_infile(t_shell *elem);
void					dup_prev_to_stdin(int prev_fd);
void					dup_next_to_stdout(int next_write);
void					child_process(t_shell *elem, t_env **env, int prev_fd,
							int next_write);
void					child_exec_or_builtin(t_shell *elem, t_env **env);
void					child_close_fds(int prev_fd, int next_read,
							int next_write);
void					open_and_dup_outfile(char *path, int append);
void					set_flags(int *flags, int append);
void					close_prev_next(int *prev_fd, int next_read,
							int next_write);
void					wait_children(int *pids, int count,
							int *last_status_ptr, int *fd_val);
void					prepare_pipe(int next_pipe[2], int need, int *next_read,
							int *next_write);
void					init_rw(t_shell *elem, int *need_next, int *next_read,
							int *next_write);
void					close_prev_next(int *prev_fd, int next_read,
							int next_write);
void					free_shell(t_shell *element);
void					create_shlvl(t_env *tail);
void					init_env_vals(t_env **head, int *i, int *shlvl);
void					change_path(t_env *temp, t_shell *elem);
void					print_cmd_list(t_cmd *head);
void					free_cmds(t_cmd *head);
void					handle_sigint(int sig_num);
void					do_infile(char *infile);
void					do_outfile(char *outfile, int append);
void					close_and_open_child(int prev_fd, int has_next,
							int *next_pipe);
void					execute_command(t_shell *elem, t_env **env);
void					perr_exit(int errnum, char *cmd);
void					exec_command(t_shell *elem, t_env **env, char **envp);
void					delete_node(t_env **env, t_env *target, t_env *prev);
void					addlast(t_env **env, t_env *add);
void					free_tokens(t_token *head);
void					print_list(t_token *head);
void					print_enum(t_token *list);
void					free_heredoc(t_heredoc *heredoc);
void					free_cmds(t_cmd *head);
void					do_commands(t_shell *elem, t_env **env, int ac,
							int fd_val);
void					sort_list(t_env *env);
void					create_and_add(t_env **env, char *str);
void					print_env(t_env *head);
void					handle_quote(char *str, int *i, int type,
							t_struct_var *structs);
void					add_arr(t_quote_vars *vars, t_struct_var *structs,
							int *i);
void					handle_slash(t_token **head, t_token **current,
							char *str, int *i);
char					*ft_strdup(char *str);
char					*ft_strdup_char(char c);
char					*ft_strdup_char(char c);
char					*add_quoted_word(char *str, int *i, int type,
							t_token **current);
char					*expand_var(char *str, t_cmd *cmd, t_env *env);
char					**ft_split(char const *s, char c);
char					*ft_strjoin(char const *s1, char const *s2);
char					*ft_strtrim(char const *s1, char const *set);
char					*ft_strchr(const char *s, int c);
char					*expand_with_quotes(char *str, t_env *env);
char					*ft_itoa(int n);
char					*ft_dup_upto(char *str, char c);
char					**create_envp(t_env *env);
size_t					ft_strlen(const char *s);
pid_t					command_fork(t_shell *elem, t_env **env, int *prev_fd);
t_env					*free_env_list_tmp(t_env *env);
t_cmd					*new_cmd_token(t_token *tokens, t_env *envp);
t_cmd					*init_cmds(t_token *tokens, int exit_code, t_env *env);
t_env					*copy_env(char *envp[]);
t_env					*create_node(char *env);
t_env					*in_env(char *str, t_env **env);
t_env					*create_env_node(char *arg, t_env **env, int flag,
							char *str);
t_token					*new_token(token_type type, char *value, int flag,
							int new_word);
t_token					*lexer(char *str, t_env *env);
t_token					*add_word(char *str, int *i);
t_token					*assign_args(t_token *tokens, t_cmd *cmds, t_env *env);
t_token					*assign_ctl_tokens(t_token *token, t_cmd *cmd,
							t_env *envp);
t_heredoc				*init_heredoc_struct(t_cmd *cmd);
token_type				find_token_type(char *str);
int						assign_concat_flag(char *str, int i, t_token **current);
void					add_arr(t_quote_vars *vars, t_struct_var *structs,
							int *i);
void					handle_slash(t_token **head, t_token **current,
							char *str, int *i);
void					fill_arr(char *str, int *i, t_quote_vars *vars);
#endif
