/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 14:59:16 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/18 15:20:51 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	delete_node(t_env **env, t_env *target, t_env *prev)
{
	if (prev == target)
		*env = target->next;
	else
		prev->next = target->next;
	free(target->key);
	free(target->pwd_copy);
	free(target->value);
	free(target);
	target = NULL;
}

int	ft_lensplit(char **split)
{
	int	i;

	i = 0;
	while (split && split[i])
		i++;
	return (i);
}

void	free_penv(char **penv)
{
	int	i;

	i = 0;
	if (!penv)
		return ;
	while (penv[i])
		free(penv[i++]);
	free(penv);
}

void	init_rw(t_shell *elem, int *need_next, int *next_read, int *next_write)
{
	*next_read = -1;
	*next_write = -1;
	if (elem->next == NULL)
		*need_next = 0;
	else
		*need_next = 1;
}

int	env_len(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		env = env->next;
		i++;
	}
	return (i);
}
