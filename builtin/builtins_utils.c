/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:56:30 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/25 22:25:33 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	correct_export(char *str)
{
	int	i;
	int	sep;
	int	len;

	i = 0;
	sep = 0;
	len = ft_strlen(str);
	if (len == 1 && str[0] == '=')
		return (-1);
	while (i < len)
	{
		if (str[i] == '=')
			sep = 1;
		else if (!sep && !ft_isalnum(str[i]) && str[i] != '_')
			return (-1);
		else if (!sep && i == 0 && ft_isdigit(str[i]))
			return (-1);
		else if (sep && i == 1 && ft_isdigit(str[i]))
			return (-1);
		i++;
	}
	return (1);
}

void	good_export(char *arg, t_env **env, char *str)
{
	t_env	*nd;
	char	*pos;

	nd = *env;
	pos = ft_dup_upto(arg, '=');
	while (nd)
	{
		if (ft_strcmp(pos, nd->key) == 0)
		{
			if (was_hidden(nd))
				break ;
			free(nd->value);
			nd->value = ft_strdup((str + 1));
			break ;
		}
		nd = nd->next;
	}
	free(pos);
	if (!nd)
	{
		nd = create_env_node(arg, 1, str);
		addlast(env, nd);
	}
}

int	do_export(t_shell *elem, t_env **env)
{
	if ((ft_lensplit(elem->command->args) >= 1))
		return (export_loop(elem, env));
	else
		sort_list(*env);
	return (0);
}

void	create_and_add(t_env **env, char *str)
{
	t_env	*node;

	node = create_env_node(str, 0, NULL);
	addlast(env, node);
}

t_env	*create_env_node(char *arg, int flag, char *str)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	if (!flag)
	{
		node->key = ft_strdup(arg);
		node->hidden = 0;
		node->pwd_copy = NULL;
		node->value = NULL;
		node->next = NULL;
	}
	else
	{
		node->key = ft_dup_upto(arg, '=');
		node->value = ft_strdup((str + 1));
		node->pwd_copy = NULL;
		node->hidden = 0;
		node->next = NULL;
	}
	return (node);
}
