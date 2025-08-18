/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:20:03 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/18 14:58:28 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_len(char **av)
{
	int	i;

	i = 0;
	while (av && av[i])
	{
		i++;
	}
	return (i);
}

char	*get_paths(t_env **env)
{
	t_env	*nd;

	nd = *env;
	while (nd)
	{
		if (ft_strncmp(nd->key, "PATH", 4) == 0)
			return (nd->value);
		nd = nd->next;
	}
	return (NULL);
}

char	*try_paths(char **split, char *comm)
{
	char	*temp;
	char	*temp2;
	int		i;

	i = 0;
	while (split[i])
	{
		temp = ft_strjoin("/", comm);
		temp2 = ft_strjoin(split[i], temp);
		if (access(temp2, F_OK) == 0)
			return (free(temp), temp2);
		free(temp2);
		free(temp);
		i++;
	}
	return (NULL);
}

char	**join_args(char *cmd, char **args)
{
	int		len;
	int		i;
	char	**final_args;

	i = 1;
	len = count_len(args) + 1;
	final_args = malloc((len + 1) * sizeof(char *));
	if (!final_args)
		return (NULL);
	final_args[0] = cmd;
	final_args[len] = NULL;
	while (i < len)
	{
		final_args[i] = args[i - 1];
		i++;
	}
	return (final_args);
}

void	exec_command(t_shell *elem, t_env **env, char **envp)
{
	char	*path;
	char	*paths;
	char	**args;
	char	**split;

	if ((access(elem->command->cmd, F_OK) == 0 && ft_strchr(elem->command->cmd,
				'/') != NULL) || ft_strncmp(elem->command->cmd, "", 1) == 0)
		path = elem->command->cmd;
	else
	{
		paths = get_paths(env);
		if (!paths)
			exit(1);
		split = ft_split(paths, ':');
		path = try_paths(split, elem->command->cmd);
		if (!path)
			perr_exit(errno, elem->command->cmd);
	}
	args = join_args(elem->command->cmd, elem->command->args);
	execve(path, args, envp);
	printf("Error: %s\n", strerror(errno));
	exit(127);
}
