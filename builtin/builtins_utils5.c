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
	ft_putstr_fd("export: '", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	return (1);
}
