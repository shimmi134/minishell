/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 04:43:52 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/20 21:44:57 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_strdup_double(char **str)
{
	char	**dup;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
		i++;
	dup = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (str[i])
	{
		dup[i] = ft_strdup(str[i]);
		i++;
	}
	dup[i] = NULL;
	return (dup);
}
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
		for (int i = 0; i < ft_lensplit(heredoc->args); i++)
			printf("Heredoc args: [%s]\n", heredoc->args[i]);
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

void	free_heredoc(t_heredoc *heredoc)
{
		int i;

	if (!heredoc)
		return ;
	free(heredoc->heredoc_delim);
	heredoc->heredoc_delim = NULL;
	free(heredoc->cmd);
	heredoc->cmd = NULL;
	if (heredoc->args)
	{
		i = 0;
		while (heredoc->args[i])
			free(heredoc->args[i++]);
		free(heredoc->args);
	}
	free(heredoc);
}

void	handle_sigint_heredoc(int sig_int)
{
	(void)sig_int;
	exit(1);
}

char	*heredoc_expand(char *str, t_env *env)
{
	int		i;
	int		j;
	char	*tmp;
	char	*tmp2;
	char	*expanded;

	i = 0;
	j = 0;
	tmp = NULL;
	expanded = NULL;
	while (str[i] != '$')
		i++;
	if (i > 0)
	{
		tmp = malloc((sizeof(char *) * i) + 1);
		i = 0;
		while (str[i] != '$')
		{
			tmp[i] = str[i];
			i++;
		}
		tmp[i] = '\0';
	}
	i++;
	if (str[i])
	{
		j = 0;
		while (str[i])
		{
			i++;
			j++;
		}
		tmp2 = malloc((sizeof(char *) * j) + 1);
		j = 0;
		while (str[j] != '$')
			j++;
		j++;
		i = 0;
		while (str[j])
		{
			tmp2[i] = str[j];
			i++;
			j++;
		}
		tmp2[i] = '\0';
		expanded = expand_var(tmp2, NULL, env);
		if (tmp2)
			free(tmp2);
		if (expanded)
		{
			if (tmp)
			{
				tmp2 = ft_strjoin(tmp, expanded);
				free(tmp);
			}
			else
				tmp2 = expanded;
			return (tmp2);
		}
		else
			return (tmp);
	}
	return (NULL);
}

int	init_heredoc(t_heredoc *hd_temp, t_env *env, t_shell *element, int *fd_val)
{
	pid_t	pid;
	int		status;
	int		ac;

	if (hd_temp->heredoc_delim)
	{
		hd_temp->heredoc_fd = read_heredoc(hd_temp, env);
		if (hd_temp->heredoc_fd == -1)
		{
			free_heredoc(hd_temp);
			return (0);
		}
		if (*fd_val != -1)
		{
			dup2(*fd_val, STDIN_FILENO);
			close(*fd_val);
			*fd_val = -1;
			pid = fork();
			if (pid == 0)
				child_exec_or_builtin(element, &env);
			waitpid(pid, &status, 0);
		}
		pid = fork();
		if (pid == 0)
		{
			if (hd_temp->heredoc_fd != -1)
			{
				dup2(hd_temp->heredoc_fd, STDIN_FILENO);
				close(hd_temp->heredoc_fd);
			}
			element->command->heredoc = 0;
			ac = count_commands(element);
			if (hd_temp->cmd)
				do_commands(element, &env, ac, *fd_val);
			free_heredoc(hd_temp);
			env = free_env_list_tmp(env);
			exit(0);
		}
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
	return (0);
}
int	read_heredoc(t_heredoc *hd_temp, t_env *env)
{
	int		status;
	char	*line;
	char	*tmp;
	pid_t	pid;
	int		delimiter_found;
	int		pipefd[2];
	t_token	*token;
	t_cmd	*cmd;

	delimiter_found = 0;
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
		while (1)
		{
			line = readline("> ");
			if (!line)
				break ;
			if (ft_strcmp(line, hd_temp->heredoc_delim) == 0)
			{
				delimiter_found = 1;
				free(line);
				break ;
			}
			if (strchr(line, '$') && !hd_temp->heredoc_quoted)
			{
				tmp = heredoc_expand(line, env);
				if (tmp)
					write(pipefd[1], tmp, strlen(tmp));
				write(pipefd[1], "\n", 1);
				free(tmp);
			}
			else
			{
				write(pipefd[1], line, strlen(line));
				write(pipefd[1], "\n", 1);
				free(line);
			}
		}
		if (!delimiter_found)
		{
			printf("bash: warning: here-document delimited by end-of-file (wanted `%s`)\n",
				hd_temp->heredoc_delim);
		}
		close(pipefd[1]);
		free_heredoc(hd_temp);
		exit(0);
	}
	close(pipefd[1]);
	signal(SIGINT, SIG_IGN);
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
	return (pipefd[0]);
}
