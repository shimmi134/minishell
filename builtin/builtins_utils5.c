/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:08:33 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/25 22:20:13 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_dup_upto(char *str, char c)
{
	int		i;
	int		j;
	char	*helper;

	i = 0;
	j = 0;
	while (str && str[i] != c)
		i++;
	helper = malloc(i + 1);
	helper[i] = '\0';
	while (j < i)
	{
		helper[j] = str[j];
		j++;
	}
	return (helper);
}

t_env	*in_env(char *str, t_env **env)
{
	t_env	*nd;

	nd = *env;
	while (nd)
	{
		if (ft_strncmp(str, nd->key, ft_strlen(str)) == 0)
			return (nd);
		nd = nd->next;
	}
	return (NULL);
}

int	export_error(char *arg)
{
	printf("export: '%s': not a valid identifier\n", arg);
	return (1);
}
/*

int* newline_order(t_shell *elem)
{
	int	j;
	int	sep;
	int	*arr;

	arr = malloc*(sizeof(int)*2);
	if (!arr)
		return (NULL);
	arr[0] = 0
	while (arr[0] < count_len(elem->command->args))
	{
		j = 0;
		sep = 0;
		while (j < ft_strlen(elem->command->args[arr[0]))
		{
			if (elem->command->args[arr[0]][j] == 'n' && !sep)
				sep = 1;
			else if (elem->command->args[arr[0]][j] == 'n')
				arr[1] = 0;
			else if (elem->command->args[arr[0]][j] == '-' && sep)
				arr[1] = 0;
			j++;
		}
		if (arr[1] == 0)
			break;
		arr[0]++;
	}
	return (arr);
}*/
