/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 13:21:28 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/30 18:30:04 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	do_unset(t_shell *elem, t_env **env)
{
	t_env	*node;
	t_env	*prev;
	int		i;

	i = 0;
	while (elem->command->args && ft_lensplit(elem->command->args) > i)
	{
		if (in_env(elem->command->args[i], env))
		{
			node = *env;
			prev = node;
			while (node)
			{
				if (unset_loop(elem, prev, env, i) == 1)
					break ;
				if (ft_strcmp(node->key, "PWD") == 0
					&& ft_strcmp(elem->command->args[i], "PWD") == 0)
					node->hidden = 1;
				prev = node;
				node = node->next;
			}
		}
		i++;
	}
	return (0);
}

int	unset_loop(t_shell *elem, t_env *prev, t_env **env, int i)
{
	t_env	*node;

	if (ft_strcmp(prev->key, (*env)->key) != 0)
		node = prev->next;
	else
		node = prev;
	if (elem->command->args[i] && ft_strcmp(elem->command->args[i],
			node->key) == 0 && ft_strcmp(node->key, "PWD") != 0)
	{
		delete_node(env, node, prev);
		return (1);
	}
	return (0);
}

int	incorr_env(t_shell *elem)
{
	char	*str;
	char	*str2;
	int		v;

	v = 0;
	str = elem->command->args[0];
	str2 = elem->command->args[1];
	if (str2)
		v = access(str2, F_OK | X_OK);
	if (str && ft_strncmp(str, "-", 1) == 0 && !str2)
		return (3);
	if (str && !str2)
		return (3);
	if (v == -1)
		return (printf("env: '%s': %s\n", str2, strerror(errno)), 2);
	return (str && ft_strncmp(str, "-i", 2) == 0 && str2 && ft_strncmp(str2,
			"./minishell", ft_strlen(str2)) == 0);
}

char	*change_pwd(t_env *temp, char *str, char *arg)
{
	char	*tstr;
	
	tstr = ft_strdup(temp->value);
	if (ft_strcmp(".", arg) != 0)
	{
		free(temp->value);
		temp->value = str;
	}
	else
		free(str);
	return (tstr);
}