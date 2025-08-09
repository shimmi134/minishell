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


void perr_exit(int errnum, char *cmd)
{
	perror(cmd);
	exit(errnum);
}
void	exec_command(t_shell *elem, t_env **env, char **envp)
{
	char	*path;
	char	*paths;
	char	**args;
	char	**split;

	if ((access(elem->command->cmd, F_OK) == 0 && ft_strchr(elem->command->cmd, '/') != NULL) || ft_strncmp(elem->command->cmd, "", 1) == 0)
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

t_env *duplicate_env(t_env* env)
{
	t_env *dup;
	t_env *head;
	t_env *last;
	
	head = NULL;
	dup = NULL;
	last = NULL;
	while (env != NULL)
	{
		dup = malloc(sizeof(t_env));
		if (!dup)
			return (NULL);
		dup->key = ft_strdup(env->key);
		dup->value = ft_strdup(env->value);
		dup->next = NULL;
		env = env->next;
		if (head == NULL)
			head = dup;
		else
			last->next = dup;
		last = dup;	
	}
	return head;
}

int swap(t_env *temp)
{
	char *key;
	char *value;

	if (ft_strncmp(temp->key, temp->next->key, ft_strlen(temp->key)) > 0)
	{
		key = temp->key;
		value = temp->value;
		temp->key = temp->next->key;
		temp->value = temp->next->value;
		temp->next->key = key;
		temp->next->value = value;
		return (1);
	}
	return (0);
}

void sort_list(t_env *env)
{
    t_env *temp;
    t_env *head;
    int swapped;

    if (env == NULL)
        return ;
    head = duplicate_env(env);
    swapped = 1;
    while (swapped) 
	{
        swapped = 0;
        temp = head;
        while (temp && temp->next) 
		{
			swapped = swap(temp);
            temp = temp->next;
        }
    }
	print_env(head);
	free_env_list_tmp(head);
}

void print_env(t_env *head)
{
	while (head != NULL)
	{
		printf("declare -x %s", head->key);
		if (head->value)
			printf("=\"%s\"", head->value);
		printf("\n");
		head = head->next;
	}
}
