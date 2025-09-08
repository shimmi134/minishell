/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 18:21:23 by shimi-be          #+#    #+#             */
/*   Updated: 2025/09/08 23:03:16 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_pipe_command(int *fd_val, t_env *env, t_shell *element)
{
	pid_t	pid;
	int		status;

	status = 0;
	if (*fd_val != -1 && element->command->cmd)
	{
		dup2(*fd_val, STDIN_FILENO);
		close(*fd_val);
		*fd_val = -1;
		pid = fork();
		if (pid == 0)
			child_exec_or_builtin(element, &env);
		waitpid(pid, &status, 0);
	}
	return (status);
}

void	execute_heredoc_command(t_heredoc *hd_temp, t_shell *element,
		t_env *env, int *fd_val)
{
	int val;

	val = 0;
	if (hd_temp->heredoc_fd != -1)
	{
		dup2(hd_temp->heredoc_fd, STDIN_FILENO);
		close(hd_temp->heredoc_fd);
	}
	element->command->heredoc_fd = -2;
	if (hd_temp->cmd)
	{
		if (!check_files(&element) &&  ( !element->next || element->next->command->heredoc != 1))
			command_fork(element, &env, fd_val);
		else if (element->next->command->heredoc != 1)
			val = 1;
	}
	free_heredoc(hd_temp);
	env = free_env_list_tmp(env);
	exit(val);
}

void	write_expand(char *line, t_env *env, int pipefd)
{
	char	*tmp;

	tmp = heredoc_expand(line, 0, env);
	if (tmp)
		write(pipefd, tmp, ft_strlen(tmp));
	write(pipefd, "\n", 1);
	free(tmp);
	free(line);
}

void	expand_or_write(char *line, t_env *env, int pipefd, t_heredoc *hd_temp)
{
	if (strchr(line, '$') && !hd_temp->heredoc_quoted)
		write_expand(line, env, pipefd);
	else
	{
		printf("line = %s\n", line);
		write(pipefd, "\n", 1);
		write(pipefd, line, ft_strlen(line));
		write(pipefd, "\n", 1);
		free(line);
	}
}

void	heredoc_loop(int pipefd[2], t_heredoc *hd_temp, t_env *env, t_shell *elem)
{
	int		delimiter_found;
	char	*line;

	delimiter_found = 0;
	while (1)
	{
	//	if (elem->command->heredoc_delim[0])
	printf("here2\n");
			line = readline("> ");
		// else
		// {
		// 	line = readline("\n> ");
		// }
		if (!line)
			break ;
		if (ft_strcmp(line, hd_temp->heredoc_delim) == 0)
		{
			delimiter_found = 1;
			free(line);
			break ;
		}
		expand_or_write(line, env, pipefd[1], hd_temp);
		// if (!elem->command->heredoc_delim[1])
		// 	dup2(pipefd, 1);
		 write(1, "\n", 1);
	}
	if (!delimiter_found)
		delimiter_message(hd_temp->heredoc_delim);
	close(pipefd[1]);
	write(1, "\n", 1);
	free_heredoc(hd_temp);
	exit(0);
}
