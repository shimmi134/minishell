/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:36:52 by shimi-be          #+#    #+#             */
/*   Updated: 2025/04/29 18:37:41 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_test.h"

void delete_node(t_env **env, t_env *target, t_env *prev)
{
	if (prev == target)
		*env = target->next;
	else
		prev->next = target->next;
	free(target);
}


void	do_builtins(t_shell *elem, t_env **env, char *av[])
{
	char	*buf;
	t_env	*nd;
	int		i;

	i = 0;
	if (elem->word == "pwd")
	{
		buf = getcwd(NULL, 0);
		if (!buf)
			perror("getcwd");
		else
			printf("%s\n", buf);
		free(buf);
	}
	else if (elem->word == "exit")
	{
		// kill(t_pid val_of_child, signal) MOst likely need to do in loop or smth
		exit(0);
	}
	else if (elem->word == "env")
	{
		nd = (*env);
		while (nd)
		{
			printf("%s=%s\n",nd->key, nd->value);
			nd = nd->next;
		}
	}
	else if (elem->word == "unset")
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
		nd = (*env);
		while (nd)
		{
			printf("%s=%s\n",nd->key, nd->value);
			nd = nd->next;
		}
	}
	else if (elem->word == "echo")
	{
	}
	else if (elem->word == "export")
	{
	}
	else if (elem->word == "cd")
	{
	}
}
void	do_element(t_shell *elem, t_env **env, char *av[])
{
	if (elem->type == "built-in")
		do_builtins(elem, env, av);
}

t_env	*create_node(char *env)
{
	char	**split;
	t_env	*node;

	split = ft_split(env, '=');
	if (!split[0] || !split[1])
		return (NULL);
	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = split[0];
	node->value = split[1];
	return (node);
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

int	main(int ac, char *argv[], char *envp[])
{
	t_shell	*element;
	t_env	*env;

	env = copy_env(envp);
	element = malloc(sizeof(t_shell));
	element->type = "built-in";
	element->word = "unset";
	do_element(element, &env, argv);
}
