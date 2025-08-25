/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 13:21:28 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/25 22:02:17 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_unset(t_shell *elem, t_env **env)
{
	t_env	*nd;
	t_env	*prev;

	nd = (*env);
	prev = nd;
	while (nd)
	{
		if (elem->command->args[0] && ft_strcmp(elem->command->args[0],
				nd->key) == 0 && ft_strcmp(nd->key, "PWD") != 0)
		{
			delete_node(env, nd, prev);
			break ;
		}
		if (ft_strcmp(nd->key, "PWD") == 0)
			nd->hidden = 1;
		prev = nd;
		nd = nd->next;
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
