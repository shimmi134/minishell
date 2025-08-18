/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:21:10 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/18 15:21:35 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_shlvl(t_env *env)
{
	char	*str;

	if (ft_atoi(env->value) >= 1000)
		env->value = ft_itoa(1);
	str = env->value;
	env->value = ft_itoa(ft_atoi(env->value) + 1);
	free(str);
}

char	*create_pair(t_env *env)
{
	char	*temp;
	char	*str;

	temp = ft_strjoin(env->key, "=");
	if (!temp)
		return (NULL);
	str = ft_strjoin(temp, env->value);
	free(temp);
	if (!str)
		return (NULL);
	return (str);
}

char	**create_envp(t_env *env)
{
	char	**penv;
	int		i;
	int		len;

	i = 0;
	len = env_len(env);
	penv = malloc(sizeof(char *) * (len + 1));
	if (!penv)
		return (NULL);
	while (i < len && env)
	{
		if (!ft_strncmp(env->key, "SHLVL=", 6))
			do_shlvl(env);
		penv[i] = create_pair(env);
		env = env->next;
		i++;
	}
	penv[i] = NULL;
	return (penv);
}

void	create_shlvl(t_env *tail)
{
	t_env	*new_node;

	new_node = create_node("SHLVL=2");
	tail->next = new_node;
	new_node->next = NULL;
}

void	init_env_vals(t_env **head, int *i, int *shlvl)
{
	*head = NULL;
	*i = 0;
	*shlvl = 0;
}
