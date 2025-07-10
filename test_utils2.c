/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:26:05 by shimi-be          #+#    #+#             */
/*   Updated: 2025/07/10 18:37:24 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_node(t_env **env, t_env *target, t_env *prev)
{
	if (prev == target)
		*env = target->next;
	else
		prev->next = target->next;
	free(target);
}

int	count_commands(t_shell *sh)
{
	int	i;

	i = 0;
	while (sh)
	{
		sh = sh->next;
		i++;
	}
	return (i);
}

void	addlast(t_env **env, t_env *add)
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
	if (!split)
		return (NULL);
	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	if (!ft_strncmp("SHLVL", split[0], ft_strlen(split[0])))
		split[1] = ft_itoa(ft_atoi(split[1])+1);
	node->key = split[0];
	char *str = split[1];
	for (int i = 2; i < ft_lensplit(split);)
	{
		str = ft_strjoin(str, split[i]); // leaks
		i++;
		if (split[i])
			str = ft_strjoin(str, "=");
	}
	node->value = str;
	return (node);
}

int	ft_strspn(char *str, char *sep)
{
	int	i;
	int	j;

	if (!str || !sep)
		return (0);
	i = 0;
	while (str[i])
	{
		j = 0;
		while (sep[j])
		{
			if (sep[j] == str[i])
				break ;
			j++;
		}
		if (!sep[j])
			return (i);
		i++;
	}
	return (i);
}
