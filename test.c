/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:36:52 by shimi-be          #+#    #+#             */
/*   Updated: 2025/04/29 17:57:03 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_test.h"

void	do_builtins(t_shell *elem, t_env *env)
{
	char	*buf;
	int		i;

	i = 0;
	if (elem->word == "pwd")
	{
		buf = getcwd(NULL, 0);
		if (!buf)
			perror("getcwd");
		else
			printf("%s\n", buf);
	}
	else if (elem->word == "exit")
	{
		// kill(t_pid val_of_child, signal) MOst likely need to do in loop or smth
		exit(0);
	}
	else if (elem->word == "env")
	{
		while (env)
		{
			printf("%s=%s\n",env->key, env->value);
			env = env->next;
		}
	}
	else if (elem->word == "unset")
	{
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
void	do_element(t_shell *elem, t_env *env)
{
	if (elem->type == "built-in")
		do_builtins(elem, env);
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

int	main(int ac, char *argv, char *envp[])
{
	t_shell	*element;
	t_env	*env;

	env = copy_env(envp);
	element = malloc(sizeof(t_shell));
	element->type = "built-in";
	element->word = "env";
	do_element(element, env);
}
