/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 18:22:24 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/25 20:58:20 by shimi-be         ###   ########.fr       */
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

void	free_heredoc(t_heredoc *heredoc)
{
	int	i;

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

void	heredoc_execute_loop(t_shell *elem, t_env **env, int *fd_val)
{
	while (elem->command->heredoc_delim[0])
	{
		do_heredoc(elem->command, *env, elem->exit_status_code, fd_val);
		shift_left_and_free(elem->command->heredoc_delim);
	}
}

void	delimiter_message(char *str)
{
	printf("bash: warning:");
	printf("here-document delimited by end-of-file (wanted `%s`)\n", str);
}
