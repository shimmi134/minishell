/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:26:05 by shimi-be          #+#    #+#             */
/*   Updated: 2025/07/18 12:50:28 by shimi-be         ###   ########.fr       */
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

t_env   *create_node(char *env)
{
    char    **split;
	char	*equal;
    t_env   *node;
    int i;

    i = 0;
	equal = ft_strchr(env, '=');
    split = ft_split(env, '=');
    if (!split)
        return (NULL);
    node = malloc(sizeof(t_env));
    if (!node)
        return (NULL);
    node->key = ft_strdup(split[0]);
    node->value = ft_strdup(split[1]);
    node->next = NULL;
    while (split[i])
        free(split[i++]);
    free(split);
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
