/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:20:03 by shimi-be          #+#    #+#             */
/*   Updated: 2025/07/10 18:38:16 by shimi-be         ###   ########.fr       */
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

	if (access(elem->command->cmd, F_OK) != 0)
	{
		paths = get_paths(env);
		if (!paths)
			exit(1);
		split = ft_split(paths, ':');
		path = try_paths(split, elem->command->cmd);
		if (!path)
		{
			perror(elem->command->cmd);
			exit(errno);
		}
	}
	else
		path = elem->command->cmd;
	args = join_args(elem->command->cmd, elem->command->args);
	execve(path, args, envp);
	printf("Error: %s\n", strerror(errno));
	exit(127);
}

int	corr_input(t_shell *elem)
{
	return !(ft_lensplit(elem->command->args) > 1);
}
t_env *sort_list(t_env **env)
{
    t_env *temp;
    t_env *head;
    int swapped;

    if (env == NULL || *env == NULL)
        return NULL;
    head = *env;
    swapped = 1;
    while (swapped) {
        swapped = 0;
        temp = head;
        while (temp && temp->next) {
            if (ft_strncmp(temp->key, temp->next->key, ft_strlen(temp->key)) > 0) {
                char *key = temp->key;
                char *value = temp->value;
                temp->key = temp->next->key;
                temp->value = temp->next->value;
                temp->next->key = key;
                temp->next->value = value;
                swapped = 1;
            }
            temp = temp->next;
        }
    }
    return head;
}
