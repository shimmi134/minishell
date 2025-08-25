/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 04:43:52 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/25 20:30:24 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_heredoc	*init_heredoc_struct(t_cmd *cmd)
{
	t_heredoc	*heredoc;

	heredoc = malloc(sizeof(t_heredoc));
	if (!heredoc)
		return (NULL);
	if (cmd->cmd)
		heredoc->cmd = ft_strdup(cmd->cmd);
	else
		heredoc->cmd = NULL;
	if (cmd->args[0])
	{
		heredoc->args = ft_strdup_double(cmd->args);
		if (!heredoc->args)
			printf("dup_double failed\n");
	}
	else
		heredoc->args = NULL;
	heredoc->heredoc_delim = ft_strdup(cmd->heredoc_delim);
	heredoc->heredoc_fd = cmd->heredoc_fd;
	heredoc->heredoc_quoted = cmd->heredoc_quoted;
	heredoc->next = NULL;
	return (heredoc);
}

char	*heredoc_expand(char *str, t_env *env)
{
	int		i;
	char	*tmp;

	i = 0;
	i = calc_dollar(str);
	tmp = copy_upto(i, str);
	i++;
	if (str[i])
		return (ret_exp(i, str, env, tmp));
	return (NULL);
}

int	init_heredoc(t_heredoc *hd_temp, t_env *env, t_shell *element, int *fd_val)
{
	pid_t	pid;
	int		status;

	status = 0;
	if (hd_temp->heredoc_delim)
	{
		hd_temp->heredoc_fd = read_heredoc(hd_temp, env);
		if (hd_temp->heredoc_fd == -1)
			return (free_heredoc(hd_temp), 0);
		status = execute_pipe_command(fd_val, env, element);
		pid = fork();
		if (pid == 0)
			execute_heredoc_command(hd_temp, element, env, fd_val);
		else if (pid > 0)
		{
			if (hd_temp->heredoc_fd != -1)
				close(hd_temp->heredoc_fd);
			signal(SIGINT, SIG_IGN);
			waitpid(pid, &status, 0);
		}
		else
			perror("fork");
	}
	free_heredoc(hd_temp);
	return (status);
}

int	wait_status(pid_t pid)
{
	int	status;

	status = 0;
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		status = WTERMSIG(status);
		if (status == SIGINT)
		{
			printf("> ^C\n");
			return (-1);
		}
	}
	return (0);
}
int	read_heredoc(t_heredoc *hd_temp, t_env *env)
{
	int		status;
	pid_t	pid;
	int		pipefd[2];

	status = 0;
	if (pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		close(pipefd[0]);
		heredoc_loop(pipefd, hd_temp, env);
	}
	close(pipefd[1]);
	signal(SIGINT, SIG_IGN);
	if (wait_status(pid) == -1)
		return (-1);
	return (pipefd[0]);
}
