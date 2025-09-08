/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils8.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:24:40 by shimi-be          #+#    #+#             */
/*   Updated: 2025/09/08 21:08:16 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	open_and_dup_outfile(char *path, int append)
{
	int	flags;
	int	fd;
	int	res;

	flags = 0;
	set_flags(&flags, append);
	fd = open(path, flags, 0644);
	if (fd < 0)
	{
		if (g_exit_code != 1)
			perror(path);
		g_exit_code = 1;
		return (1);
	}
	ft_putstr_fd("OPENANDDUP\n", 2);
	res = dup2(fd, STDOUT_FILENO);
	if (res < 0)
	{
		if (g_exit_code != 1)
			perror(path);
		g_exit_code = 1;
		return (1);
	}
	close(fd);
	return (0);
}

int	run_builtin(t_shell *elem, t_env **env)
{
	int	code;

	code = 0;
	if (elem->command->outfile)
		if (open_and_dup_outfile(elem->command->outfile,
				elem->command->append) == 1)
			return (1);
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
			g_exit_code = 1;
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
		ft_putstr_fd("DUPPREVTO\n",2 );
		res = dup2(prev_fd, STDIN_FILENO);
		if (res < 0)
		{
			perror("dup2");
			exit(1);
		}
	}
}
