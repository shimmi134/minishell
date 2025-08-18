/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils7.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:22:28 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/18 15:43:22 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_builtins(t_shell *elem, t_env **env)
{
	if (!ft_strncmp(elem->command->cmd, "pwd", 3))
		return (do_pwd());
	else if (!ft_strncmp(elem->command->cmd, "env", 3))
		return (do_env(env));
	else if (!ft_strncmp(elem->command->cmd, "unset", 5))
		return (do_unset(elem, env));
	else if (!ft_strncmp(elem->command->cmd, "echo", 4))
		return (do_echo(elem, env));
	else if (!ft_strncmp(elem->command->cmd, "export", 6))
		return (do_export(elem, env));
	else if (!ft_strncmp(elem->command->cmd, "cd", 2))
		return (do_cd(elem, env));
	return (0);
}

int	if_valid(char *str)
{
	int	len;

	len = ft_strlen(str);
	if (len == 3)
	{
		if (str[0] == '\"' || str[0] == '\'' && (str[2] == '\"'
				|| str[2] == '\'' && str[2] == '\''))
			return (printf("%c: command not found\n", str[1]), 0);
	}
	else if (len == 2)
	{
		if (str[0] == '\"' || str[0] == '\'' && (str[1] == '\"'
				|| str[1] == '\'' && str[1] == '\''))
			return (0);
	}
	return (1);
}

void	parent_housekeeping(int *prev_fd, int next_read, int next_write)
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

void	child_handle_infile(t_shell *elem)
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

void	child_handle_outfile(t_shell *elem)
{
	int	fd;
	int	flags;

	fd = -1;
	flags = 0;
	if (elem->command->outfile)
	{
		set_out_flags(&flags, elem->command->append);
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
