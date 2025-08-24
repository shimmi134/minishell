/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils9.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:25:13 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/24 16:35:22 by shimi-be         ###   ########.fr       */
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

void	child_close_fds(int prev_fd, int next_read, int next_write)
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
	if (elem->command->cmd != NULL && ft_strncmp(elem->type, "built-in",
			ft_strlen(elem->type)) != 0)
	{
		penv = create_envp(*env);
		exec_command(elem, env, penv);
	}
	else
	{
		if (ft_strncmp("exit", elem->command->cmd, 4) == 0)
			do_exit();
		code = do_builtins(elem, env);
		exit(code);
	}
}

void	child_process(t_shell *elem, t_env **env, int prev_fd, int next_write)
{
	dup_prev_to_stdin(prev_fd);
	dup_next_to_stdout(next_write);
	child_close_fds(prev_fd, -1, next_write);
	child_infile(elem);
	child_outfile(elem);
	child_exec_or_builtin(elem, env);
}

void	wait_children(int *pids, int count, int *last_status_ptr, int *fd_val)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	if (*fd_val != -1)
		close(*fd_val);
	while (i < count)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
		{
			if (last_status_ptr)
				*last_status_ptr = WEXITSTATUS(status);
		}
		else
		{
			if (last_status_ptr)
				*last_status_ptr = 127;
		}
		i = i + 1;
	}
	free(pids);
}
