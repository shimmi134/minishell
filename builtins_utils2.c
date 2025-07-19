/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 13:21:28 by shimi-be          #+#    #+#             */
/*   Updated: 2025/07/19 13:26:52 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int do_unset(t_shell *elem, t_env **env)
{
	t_env *nd;
	t_env *prev;

	nd = (*env);
	prev = nd;
	while (nd)
	{
		if (!ft_strncmp(elem->command->args[0], nd->key,
				ft_strlen(elem->command->args[0])))
		{
			delete_node(env, nd, prev);
			break ;
		}
		prev = nd;
		nd = nd->next;
	}
	return (0);
}
