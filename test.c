/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:36:52 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/19 17:29:11 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

pid_t	spawn_one_process(t_shell *elem, t_env **env, int *prev_fd)
{
	int		next_pipe[2];
	pid_t	pid;
	int		need_next;
	int		next_read;
	int		next_write;

	init_rw(elem, &need_next, &next_read, &next_write);
	next_pipe[0] = -1;
	next_pipe[1] = -1;
	prepare_pipe_if_needed(next_pipe, need_next);
	if (need_next)
		next_read = next_pipe[0];
	if (need_next)
		next_write = next_pipe[1];
	pid = fork();
	if (pid < 0)
		perror("fork");
	if (pid < 0)
		exit(1);
	if (pid == 0)
	{
		if (next_read != -1)
			close(next_read);
		child_process(elem, env, *prev_fd, next_write);
		exit(1);
	}
	parent_housekeeping(prev_fd, next_read, next_write);
	return (pid);
}

void	wait_children_and_set_status(int *pids, int count,
		int *last_status_ptr)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < count)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
		{
			if (last_status_ptr)
			{
				*last_status_ptr = WEXITSTATUS(status);
			}
		}
		else
		{
			if (last_status_ptr)
			{
				*last_status_ptr = 127;
			}
		}
		i = i + 1;
	}
}

int	execute_loop(t_shell *elem, t_env **env, int *pids,
		int **last_status_ptr_out)
{
	pid_t	pid;
	int 	prev_fd;
	int		count;

	prev_fd = -1;
	count = 0;
	while (elem && elem->type)
	{
		if (strncmp(elem->type, "built-in", 9) == 0 && elem->next == NULL)
		{
			run_standalone_builtin(elem, env);
			*last_status_ptr_out = elem->exit_status_code;
			elem = elem->next;
			continue ;
		}
		pid = spawn_one_process(elem, env, &prev_fd);
		pids[count] = (int)pid;
		count++;
		*last_status_ptr_out = elem->exit_status_code;
		elem = elem->next;
	}
	if (prev_fd != -1)
		close(prev_fd);
	return (count);
}

void	do_commands(t_shell *elem, t_env **env, int ac)
{
	int	*pids;
	int	old_stdout;
	int	*last_status_ptr;
	int	count;

	pids = NULL;
	old_stdout = -1;
	last_status_ptr = NULL;
	count = 0;
	pids = calloc(ac + 1, sizeof(int));
	old_stdout = dup(STDOUT_FILENO);
	count = execute_loop(elem, env, pids, &last_status_ptr);
	wait_children_and_set_status(pids, count, last_status_ptr);
	if (old_stdout != -1)
		dup2(old_stdout, STDOUT_FILENO);
	if (old_stdout != -1)
		close(old_stdout);
	free(pids);
}

int	main(int argc, char *argv[], char *envp[])
{
	char		*line;
	t_shell		*element;
	t_token		*node;
	t_token		*head;
	t_cmd		*t_head;
	t_env		*env;
	t_cmd		*hd_temp;
	int			hd_res;
	int			*exit_status;
	t_heredoc	*heredoc;

	head = NULL;
	t_head = NULL;
	if (!envp || envp[0] == NULL)
		return (printf("Error, no env detected.\n"), 1);
	env = copy_env(envp);
	if (!env)
		return (printf("Error, copying the env.\n"), 1);
	element = NULL;
	exit_status = (int *)malloc(sizeof(int));
	*exit_status = 0;
	while (1)
	{
		head = NULL;
		t_head = NULL;
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		line = readline("\033[1;34mminishell>\033[0m ");
		if (line == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (line && *line)
			add_history(line);
		if (line != NULL && check_quotes(head, line))
		{
			node = lexer(line, env);
			head = node;
		//	print_list(node);
			if (check_tokens(head))
			{
				t_head = init_cmds(node, env);
				if (ft_strcmp(t_head->cmd, "exit") == 0)
				{
					free_tokens(head);
					free_cmds(t_head);
					break ;
				}
				hd_temp = t_head;
				if (hd_temp->heredoc_delim)
				{
					heredoc = init_heredoc_struct(hd_temp);
					free_tokens(head);
					clear_history();
					do_struct(&element, t_head, exit_status);
					hd_res = init_heredoc(heredoc, env, line, element);
					free(exit_status);
					exit_status = malloc(sizeof(int));
					if (!exit_status)
						return (1);
					*exit_status = hd_res;
					free_cmds(t_head);
					head = NULL;
					t_head = NULL;
					line = NULL;
					env = copy_env(envp);
				}
				else
				{
					do_struct(&element, t_head, exit_status);
					argc = count_commands(element);
					do_commands(element, &env, argc);
				}
			}
			if (line)
				rl_free(line);
		}
		if (element != NULL)
		{
			free_shell(element);
			element = NULL;
		}
		if (head)
			free_tokens(head);
		if (t_head)
			free_cmds(t_head);
	}
	if (exit_status)
		free(exit_status);
	if (line)
		rl_free(line);
	if (env)
		env = free_env_list_tmp(env);
	clear_history();
	return (0);
}
