/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils14.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:43:04 by shimi-be          #+#    #+#             */
/*   Updated: 2025/09/08 19:19:09 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_out(t_shell *elem, int m)
{
	int	fd;
	int	flags;
	int	p;

	flags = 0;
	p = m;
	if (elem->command->outfile && !p)
	{
		set_flags(&flags, elem->command->append);
		fd = open(elem->command->outfile, flags, 0644);
		if (fd < 0 && !p && !g_exit_code)
		{
			g_exit_code = 1;
			perror(elem->command->outfile);
			p = 1;
		}
		close(fd);
	}
	return (p);
}

int	check_in(t_shell *elem, int m)
{
	int	fd;
	int	flags;
	int	p;

	flags = 0;
	p = m;
	if (elem->command->infile)
	{
		fd = open(elem->command->infile, O_RDONLY);
		if (fd < 0 && !p && !g_exit_code)
		{
			g_exit_code = 1;
			perror(elem->command->infile);
			p = 1;
		}
		close(fd);
	}
	return (p);
}

int	check_files(t_shell **elem)
{
	int	flag;

	flag = check_out_in(*elem);
	while (flag)
	{
		(*elem) = (*elem)->next;
		while (*elem && !(*elem)->command->pipe)
			(*elem) = (*elem)->next;
		if (*elem)
			flag = check_out_in(*elem);
		else
			break ;
	}
	if (flag)
		return (1);
	return (0);
}

int	early_break(t_shell *elem, t_env **env, int *fd_val)
{
	if (elem->command->heredoc && elem->command->heredoc_delim)
	{
		heredoc_execute_loop(elem, env, fd_val);
		return (1);
	}
	if (check_files(&elem))
		return (1);
	return (0);
}

void	init_next_pipe(int next_pipe[2])
{
	next_pipe[0] = -1;
	next_pipe[1] = -1;
}
