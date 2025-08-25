/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils8.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:24:40 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/25 16:22:24 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_flags(int *flags, int append)
{
	*flags = 0;
	if (append)
	{
		*flags = O_WRONLY | O_CREAT | O_APPEND;
		return ;
	}
	*flags = O_WRONLY | O_CREAT | O_TRUNC;
}

void	open_and_dup_outfile(char *path, int append)
{
	int	flags;
	int	fd;
	int	res;

	flags = 0;
	fd = -1;
	res = 0;
	set_flags(&flags, append);
	fd = open(path, flags, 0644);
	if (fd < 0)
	{
		perror(path);
		exit(1);
	}
	res = dup2(fd, STDOUT_FILENO);
	if (res < 0)
	{
		perror(path);
		exit(1);
	}
	close(fd);
}

int	run_builtin(t_shell *elem, t_env **env)
{
	int	code;

	code = 0;
	if (elem->command->outfile)
		open_and_dup_outfile(elem->command->outfile, elem->command->append);
	code = do_builtins(elem, env);
	if (elem->exit_status_code)
		*(elem->exit_status_code) = code;
	return (code);
}

void	prepare_pipe(int next_pipe[2], int need, int *next_read,
		int *next_write)
{
	int	res;

	res = 0;
	next_pipe[0] = -1;
	next_pipe[1] = -1;
	if (need)
	{
		res = pipe(next_pipe);
		if (res < 0)
		{
			perror("pipe");
			exit(1);
		}
	}
	if (need)
		*next_read = next_pipe[0];
	if (need)
		*next_write = next_pipe[1];
}

void	dup_prev_to_stdin(int prev_fd)
{
	int	res;

	res = 0;
	if (prev_fd != -1)
	{
		res = dup2(prev_fd, STDIN_FILENO);
		if (res < 0)
		{
			perror("dup2");
			exit(1);
		}
	}
}
