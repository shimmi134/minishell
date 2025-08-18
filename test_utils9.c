/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils9.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:25:13 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/18 15:42:42 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dup_next_to_stdout(int next_write)
{
	int	res;

	res = 0;
	if (next_write != -1)
	{
		res = dup2(next_write, STDOUT_FILENO);
		if (res < 0)
		{
			perror("dup2");
			exit(1);
		}
	}
}

void	child_close_unneeded_fds(int prev_fd, int next_read, int next_write)
{
	if (prev_fd != -1)
		close(prev_fd);
	if (next_read != -1)
		close(next_read);
	if (next_write != -1)
		close(next_write);
}

void	child_exec_or_builtin(t_shell *elem, t_env **env)
{
	char	**penv;
	int		code;

	penv = NULL;
	if (elem->command->cmd != NULL)
	{
		penv = create_envp(*env);
		exec_command(elem, env, penv);
		perror("exec");
		exit(1);
	}
	else
	{
		code = do_builtins(elem, env);
		exit(code);
	}
}

void	child_process(t_shell *elem, t_env **env, int prev_fd, int next_write)
{
	dup_prev_to_stdin(prev_fd);
	dup_next_to_stdout(next_write);
	child_close_unneeded_fds(prev_fd, -1, next_write);
	child_handle_infile(elem);
	child_handle_outfile(elem);
	child_exec_or_builtin(elem, env);
}
