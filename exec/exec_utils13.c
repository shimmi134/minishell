/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils13.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 16:24:06 by shimi-be          #+#    #+#             */
/*   Updated: 2025/09/05 20:04:54 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	no_path_error(t_shell *elem)
{
	ft_putstr_fd(elem->command->cmd, 2);
	ft_putstr_fd(": no such file or directory\n", 2);
	exit(127);
}

t_env	*dup_node(t_env *env)
{
	t_env	*dup;

	dup = malloc(sizeof(t_env));
	if (!dup)
		return (NULL);
	dup->key = ft_strdup(env->key);
	dup->value = ft_strdup(env->value);
	dup->pwd_copy = ft_strdup(env->pwd_copy);
	dup->next = NULL;
	return (dup);
}

void	check_code(int *exit_status)
{
	if (g_exit_code)
	{
		*exit_status = g_exit_code;
		g_exit_code = 0;
	}
}
