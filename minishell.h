/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:05:53 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/23 16:00:00 by shimi-be         ###   ########.fr       */
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

typedef enum s_type
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
}						t_type;

typedef struct s_token
{
	t_type				type;
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
void					allocate_word_arr(char **arr, char *str, int *i, int j);
void					allocate_after_single(char *str, t_quote_vars *vars,
							int *i);
int						is_word(char *str);
int						is_token(char c);
void					handle_nested_var(t_struct_var *structs,
							t_quote_vars *vars, char *str, int *i);
void					flush_arr_in_var(t_quote_vars *vars,
							t_struct_var *structs);
void					handle_nested_slash(t_struct_var *structs,
							t_quote_vars *vars, char *str, int *i);
void					handle_nested_single(t_struct_var *structs,
							t_quote_vars *vars, char *str, int *i);
int						check_tokens(t_token *tokens);
int						arg_count(t_token *tokens);
void					allocate_arr(t_quote_vars *vars);
void					assign_after_var(char *str, t_quote_vars *vars, int *i);
t_token					*assign_word_arr(char *arr, int new_word);
int						token_loop(t_token *tokens);
int						token_loop_if(t_token *tokens);

int						ft_strncmp(const char *s1, const char *s2, size_t n);
int						ft_strcmp(char *s1, char *s2);
void					assign_var_token(t_struct_var *structs,
							t_quote_vars *vars, char *str, int *i);
void					assign_arr_memory(t_quote_vars *vars);
void					add_slash(t_struct_var *structs, t_quote_vars *vars);
void					add_single(t_struct_var *structs, t_quote_vars *vars);
int						check_quotes(char *str);
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
int						do_echo(t_shell *elem);
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
int						init_execute(t_token *node, t_token *head, t_env *env,
							int *exit_status);
int						pre_exec(char *line, t_env *env, int *exit_status);
int						check_line(char *str);
int						execute_pipe_command(int *fd_val, t_env *env,
							t_shell *element);
int						calc_dollar(char *str);
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
void					if_not_token(char *str, t_token **head,
							t_token **current, int *i);
void					init_token_flags(t_token *token, int new_word, int type,
							int quote);
void					handle_var_assign(char *value, t_token *token,
							int quote);
void					open_and_dup_outfile(char *path, int append);
t_token					*handle_no_quote(char *str, t_token *head,
							t_token **current, int *i);
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
void					handle_single(t_quote_vars *vars, int *i, char *str);
void					print_cmd_list(t_cmd *head);
t_token					*cmd_loop(t_token *tokens, t_cmd *cmds, int type,
							t_env *envp);
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
void					do_commands(t_shell *elem, t_env **env, int fd_val);
void					sort_list(t_env *env);
void					create_and_add(t_env **env, char *str);
void					print_env(t_env *head);
char					quote_type(int type);
void					handle_quote(char *str, int *i, int type,
							t_struct_var *structs);
void					handle_empty_quotes(int *i, int new_word,
							t_token **head, t_token **current);
void					add_arr(t_quote_vars *vars, t_struct_var *structs);
void					handle_slash(t_token **head, t_token **current,
							char *str, int *i);
void					heredoc_loop(int pipefd[2], t_heredoc *hd_temp,
							t_env *env);
void					expand_or_write(char *line, t_env *env, int pipefd,
							t_heredoc *hd_temp);
void					write_expand(char *line, t_env *env, int pipefd);
void					execute_heredoc_command(t_heredoc *hd_temp,
							t_shell *element, t_env *env, int *fd_val);
void					delimiter_message(char *str);
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
char					*copy_upto(int i, char *str);
char					*copy_from_var(int i, char *str);
char					*ret_exp(int i, char *str, t_env *env, char *tmp);
char					*ft_dup_upto(char *str, char c);
char					*heredoc_expand(char *str, t_env *env);
char					**create_envp(t_env *env);
char					**ft_strdup_double(char **str);
size_t					ft_strlen(const char *s);
pid_t					command_fork(t_shell *elem, t_env **env, int *prev_fd);
t_env					*free_env_list_tmp(t_env *env);
t_cmd					*new_cmd_token(t_token *tokens);
t_cmd					*init_cmds(t_token *tokens, int exit_code, t_env *env);
t_env					*copy_env(char *envp[]);
t_env					*create_node(char *env);
t_env					*in_env(char *str, t_env **env);
t_env					*create_env_node(char *arg, int flag, char *str);
t_token					*handle_redirect(t_cmd *cmd, t_token *token, int type);
t_token					*handle_heredoc(t_cmd *cmd, t_token *token);
void					handle_varible(t_cmd *cmd, t_token *token, t_env *envp);
t_cmd					*handle_pipes(t_cmd *cmds, t_token *tokens);
t_token					*new_token(t_type type, char *value, int flag,
							int new_word);
t_token					*lexer(char *str, t_env *env);
t_token					*add_word(char *str, int *i);
t_token					*assign_args(t_token *tokens, t_cmd *cmds);
void					quote_if(char *str, t_token **head, t_token **current,
							int *i);
void					add_quoted_word_2(char *str, int *i, int type,
							t_struct_var *structs);
void					quoted_arr_len(char *str, t_quote_vars *vars);
void					init_quoted_vars(t_quote_vars *vars);
void					free_and_null(char **str);
void					free_quoted_vars(t_quote_vars **vars);
int						init_quote_vars(char **arr, int *quote, int type,
							int *i);
void					flush_arr_in_single(t_struct_var *structs,
							t_quote_vars *vars);
void					handle_quote(char *str, int *i, int type,
							t_struct_var *structs);
void					fill_arr(char *str, int *i, t_quote_vars *vars);
void					handle_double(t_quote_vars *vars, int *i, char *str,
							t_struct_var *structs);
void					handle_slash(t_token **head, t_token **current,
							char *str, int *i);
void					add_arr_to_head(t_struct_var *structs,
							t_quote_vars *vars);
void					flush_to_head(t_struct_var *structs,
							t_quote_vars *vars);
t_token					*assign_ctl_tokens(t_token *token, t_cmd *cmd,
							t_env *envp);
t_heredoc				*init_heredoc_struct(t_cmd *cmd);
t_type					find_token_type(char *str);
int						find_char_pos(char *str, char c);
void					shift_left(char **arr);
void					handle_join(t_cmd *cmds, int i);
int						assign_concat_flag(char *str, int i, t_token **current);
void					type_if(int type, t_cmd *cmds, t_token *tokens,
							t_env *envp);
void					handle_slash(t_token **head, t_token **current,
							char *str, int *i);
void					fill_arr(char *str, int *i, t_quote_vars *vars);
#endif
