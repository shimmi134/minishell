/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils7.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:22:28 by shimi-be          #+#    #+#             */
/*   Updated: 2025/09/04 21:21:30 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	do_builtins(t_shell *elem, t_env **env)
{
	if (!ft_strcmp(elem->command->cmd, "pwd"))
		return (do_pwd(env));
	else if (!ft_strcmp(elem->command->cmd, "env"))
		return (do_env(env, elem));
	else if (!ft_strcmp(elem->command->cmd, "unset"))
		return (do_unset(elem, env));
	else if (!ft_strcmp(elem->command->cmd, "echo"))
		return (do_echo(elem));
	else if (!ft_strcmp(elem->command->cmd, "export"))
		return (do_export(elem, env));
	else if (!ft_strcmp(elem->command->cmd, "cd"))
		return (do_cd(elem, env));
	return (127);
}

void	close_prev_next(int *prev_fd, int next_read, int next_write)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (next_write != -1)
		close(next_write);
	if (next_read != -1)
		*prev_fd = next_read;
	else
		*prev_fd = -1;
}

void	child_infile(t_shell *elem)
{
	int	fd;
	int	res;

	fd = -1;
	res = 0;
	if (elem->command->infile)
	{
		fd = open(elem->command->infile, O_RDONLY);
		if (fd < 0)
		{
			perror(elem->command->infile);
			exit(1);
		}
		res = dup2(fd, STDIN_FILENO);
		if (res < 0)
		{
			perror(elem->command->infile);
			exit(1);
		}
		close(fd);
	}
}

void	child_outfile(t_shell *elem)
{
	int	fd;
	int	flags;

	fd = -1;
	flags = 0;
	if (elem->command->outfile)
	{
		set_flags(&flags, elem->command->append);
		fd = open(elem->command->outfile, flags, 0644);
		if (fd < 0)
		{
			perror(elem->command->outfile);
			exit(1);
		}
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			perror(elem->command->outfile);
			exit(1);
		}
		close(fd);
	}
}

int	pre_start_check(int ac, char **av, char **ep)
{
	if (ac != 1)
		return (ft_putstr_fd("Please only run the executable.\n",2), 0);
	(void)av;
	if (!ep || !ep[0])
		return (ft_putstr_fd("Error, no env detected.\n",2), 0);
	return (1);
}
