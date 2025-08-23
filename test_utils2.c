/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:26:05 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/23 21:47:51 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*join_split(char **split)
{
	int		i;
	int		first;
	char	*str;
	char	*temp;
	char	*str2;

	i = 1;
	str = ft_strdup("");
	first = 1;
	while (i < ft_lensplit(split))
	{
		str2 = str;
		if (!first)
			str = ft_strjoin(str, "=");
		if (!first)
			free(str2);
		temp = str;
		str2 = split[i];
		str = ft_strjoin(str, str2);
		free(temp);
		i++;
	}
	return (str);
}

t_env	*create_node(char *env)
{
	char	**split;
	t_env	*node;
	int		i;

	i = 0;
	split = ft_split(env, '=');
	if (!split)
		return (NULL);
	if (ft_strncmp(split[0], "SHLVL", 5) == 0)
	{
		split[1] = correct_shlvl(split[1]);
	}
	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(split[0]);
	node->value = join_split(split);
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
