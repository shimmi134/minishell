/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:36:52 by shimi-be          #+#    #+#             */
/*   Updated: 2025/05/09 18:28:15 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <sys/wait.h>
#include "minishell_test.h"
#include <readline/readline.h>
#include <errno.h>

void	delete_node(t_env **env, t_env *target, t_env *prev)
{
	if (prev == target)
		*env = target->next;
	else
		prev->next = target->next;
	free(target);
}

void	addlast(t_env **env, t_env *add)
{
	t_env	*temp;

	temp = *env;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = add;
}

t_env	*create_node(char *env)
{
	char	**split;
	t_env	*node;

	split = ft_split(env, '=');
	if (!split)
		return (NULL);
	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = split[0];
	node->value = split[1];
	return (node);
}

void	do_builtins(t_shell *elem, t_env **env, char *av[], int ac)
{
	char	*buf;
	t_env	*nd;
	int		i;
	t_env	*prev;
		int newline;
	char	**split;
	t_env	*node;
	t_env	*tmp;
	char	*str;
	t_env	*temp;

	i = 0;
	if (!ft_strncmp(elem->word, "pwd", 3))
	{
		buf = getcwd(NULL, 0);
		if (!buf)
			perror("getcwd");
		else
			printf("%s\n", buf);
		free(buf);
	}
	else if (!ft_strncmp(elem->word, "kill", 4))
	{
		// kill(t_pid val_of_child, signal) MOst likely need to do in loop or smth
		exit(0);
	}
	else if (!ft_strncmp(elem->word, "env", 3))
	{
		nd = (*env);
		while (nd)
		{
			if (nd->value != NULL)
				printf("%s=%s\n", nd->key, nd->value);
			nd = nd->next;
		}
	}
	else if (!ft_strncmp(elem->word, "unset", 5))
	{
		nd = (*env);
		prev = nd;
		while (nd)
		{
			if (!ft_strncmp(av[1], nd->key, ft_strlen(av[1])))
			{
				delete_node(env, nd, prev);
				break ;
			}
			prev = nd;
			nd = nd->next;
		}
	}
	else if (!ft_strncmp(elem->word, "echo", 4))
	{
		newline = 1;
		i = 1;
		while (i < ac && !ft_strncmp(av[i], "-n", 3))
		{
			newline = 0;
			i++;
		}
		while (i < ac)
		{
			printf("%s", av[i]);
			if (i < ac - 1)
				printf(" ");
			i++;
		}
		if (newline)
			printf("\n");
	}
	else if (!ft_strncmp(elem->word, "export", 6))
	{
		split = ft_split(av[1], '=');
		node = create_node(av[1]);
		if (split)
		{
			if (split[0][(int)ft_strlen(split[0]) - 1] != '+')
			{
				addlast(env, node);
			}
			else
			{
				nd = (*env);
				split[0] = ft_strtrim(split[0], "+");
				while (nd != NULL)
				{
					if (!ft_strncmp(nd->key, split[0], ft_strlen(split[0])))
					{
						nd->value = ft_strjoin(nd->value, split[1]);
						break ;
					}
					nd = nd->next;
				}
				if (nd == NULL)
				{
					node->key = split[0];
					node->value = split[1];
					addlast(env, node);
				}
			}
		}
		else
		{
			tmp = *env;
			while (tmp)
			{
				printf("declare -x %s", tmp->key);
				if (tmp->value)
					printf("=%s", tmp->value);
				printf("\n");
				tmp = tmp->next;
			}
		}
	}
	else if (!ft_strncmp(elem->word, "cd", 2))
	{
		i = chdir(av[1]);
		if (i != -1)
		{
			str = getcwd(NULL, 0);
			temp = *env;
			while (temp)
			{
				if (!ft_strncmp(temp->key, "PWD", 3))
					temp->value = str;
				temp = temp->next;
			}
		}
		// return (i);
	}
}

char	*get_paths(t_env **env)
{
	t_env *nd;

	nd = *env;
	while (nd)
	{
		if (ft_strncmp(nd->key, "PATH", 4) == 0)
			return (nd->value);
		nd = nd->next;
	}
	return (NULL);
}

char	*try_paths(char	**split, char *comm)
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
			return free(temp), temp2;
		free(temp2);
		free(temp);
		i++;
	}
}

void exec_command(t_shell *elem, t_env **env, char** av, int ac, char **envp)
{
	char	*path;
	char	*paths;
	char	**split;

	paths = get_paths(env);
	if (!paths)
		exit(1);
	split = ft_split(paths, ':');
	path = try_paths(split, elem->word);
	if (!paths)
		exit(1);
	execve(path, av, envp);
	printf("Error: %s\n", strerror(errno));
}

void	do_commands(t_shell *elem, t_env **env, char** av, int ac, char **envp)
{
	int id;

	id = fork();
	if (id == -1)
	{
		perror("fork: ");
		exit(1);
	}
	if (id == 0) // child
		exec_command(elem, env, av, ac, envp);
	else
		wait(NULL);
}

void	do_element(t_shell *elem, t_env **env, char *av[], int ac, char**envp)
{
	if (!ft_strncmp(elem->type, "built-in", ft_strlen(elem->type)))
		do_builtins(elem, env, av, ac);
	else if (!ft_strncmp(elem->type, "command", ft_strlen(elem->type)))
		do_commands(elem, env, av , ac, envp);
}

t_env	*copy_env(char *envp[])
{
	t_env	*head;
	t_env	*node;
	t_env	*nnode;
	int		i;

	head = create_node(envp[0]);
	if (!head)
		return (NULL);
	i = 2;
	node = create_node(envp[1]);
	if (!node)
		return (NULL);
	head->next = node;
	while (envp[i])
	{
		nnode = create_node(envp[i]);
		if (!nnode)
			return (NULL);
		node->next = nnode;
		node = node->next;
		i++;
	}
	return (head);
}

int	count_len(char **sp)
{
	int	i;

	i = 0;
	while (sp[i])
		i++;
	return (i);
}

char	*get_element(char *line)
{
	char **split = ft_split(line, ' ');
	if (!ft_strncmp(split[0], "pwd", 4) || !ft_strncmp(split[0], "kill", 5)
		|| !ft_strncmp(split[0], "env", 4) || !ft_strncmp(split[0], "unset",
			6) || !ft_strncmp(split[0], "echo", 5) || !ft_strncmp(split[0],
			"export", 6) || !ft_strncmp(split[0], "cd ", 3))
	{
		return ("built-in");
	}
	else{
		return ("command");
	}
}
// TODO:
// Make it so I transform all of line into nodes of t_shell
//
int	main(int ac, char *argv[], char *envp[])
{
	t_shell	*element;
	t_env	*env;
	char	*line;
	char	**split;

	env = copy_env(envp);
	split = argv;
	while (1)
	{
		line = readline(">>> ");
		if (ft_strncmp(line, "", 1))
		{
			element = malloc(sizeof(t_shell));
			element->type = get_element(line);
			split = ft_split(line, ' ');
			ac = count_len(split);
			element->word = split[0];
			do_element(element, &env, split, ac, envp);
		}
	}
}
