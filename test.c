/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:36:52 by shimi-be          #+#    #+#             */
/*   Updated: 2025/05/06 18:38:07 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_test.h"
#include <readline/readline.h>

void delete_node(t_env **env, t_env *target, t_env *prev)
{
	if (prev == target)
		*env = target->next;
	else
		prev->next = target->next;
	free(target);
}

void addlast(t_env **env, t_env *add)
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
	if (!split || !split[0] || !split[1])
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
			printf("%s=%s\n",nd->key, nd->value);
			nd = nd->next;
		}
	}
	else if (!ft_strncmp(elem->word, "unset", 5))
	{
		t_env	*nd = (*env);
		t_env	*prev = nd;
		while (nd)
		{
			if (!ft_strncmp(av[1],nd->key,ft_strlen(av[1])))
			{
				delete_node(env, nd, prev);
				break ;
			}
			prev = nd;
			nd = nd->next;
		}
	}
	else if (!ft_strncmp(elem->word,"echo",4))
	{
		int newline;

		newline = 1;
		i = 1;
		while (i < ac && !ft_strncmp(av[i], "-n", 3))
		{
			newline = 0;
			i++;
		}
		while (i < ac)
		{
			printf("%s",av[i]);
			if (i < ac-1)
				printf(" ");
			i++;
		}
		if (newline)
			printf("\n");
	}
	else if (!ft_strncmp(elem->word, "export", 6))
	{
		char **split = ft_split(av[1], '=');
		t_env *node = create_node(av[1]);
		

		if (split[0][(int)ft_strlen(split[0])-1] != '+')
		{
			addlast(env, node);
		}
		else
		{
			t_env *nd = (*env);
			split[0] = ft_strtrim(split[0], "+");
			while (nd != NULL)
			{
				if (!ft_strncmp(nd->key,split[0], ft_strlen(split[0])))
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
				addlast(env,node);
			}
		}
	}
	else if (!ft_strncmp(elem->word, "env", 3))
	{
	}
}
void	do_element(t_shell *elem, t_env **env, char *av[],int ac)
{
	if (!ft_strncmp(elem->type, "built-in", ft_strlen(elem->type)))
		do_builtins(elem, env, av, ac);
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

int count_len(char **sp)
{
	int i = 0;
	while (sp[i])
		i++;
	return i;
}

int	main(int ac, char *argv[], char *envp[])
{
	t_shell	*element;
	t_env	*env;
	char	*line;
	char	**split;

	env = copy_env(envp);
	split = ft_split(argv[0], ' ');
	while (1)
	{
		line = readline(">>> ");
		element = malloc(sizeof(t_shell));
		element->type = "built-in";
		split = ft_split(line, ' ');
		ac = count_len(split);
		element->word = split[0];
		do_element(element, &env, split, ac);
	}
}
