/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 18:19:05 by shimi-be          #+#    #+#             */
/*   Updated: 2025/09/04 20:13:05 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*copy_upto(int i, char *str)
{
	char	*tmp;

	tmp = NULL;
	if (i > 0)
	{
		tmp = malloc((sizeof(char *) * i) + 1);
		i = 0;
		while (str[i] != '$')
		{
			tmp[i] = str[i];
			i++;
		}
		tmp[i] = '\0';
	}
	return (tmp);
}

char	*copy_from_var(int *k, char *str)
{
	char	*tmp2;
	int		j;
	int		i;

	i = *k;
	j = 0;
	while (str[i] && str[i] != ' ' && str[i] != '$')
	{
		i++;
		j++;
	}
	tmp2 = malloc((sizeof(char *) * j) + 1);
	j = calc_dollar(str);
	j++;
	i = 0;
	while (str[j] && str[j] != ' ' && str[j] != '$')
	{
		tmp2[i] = str[j];
		i++;
		j++;
	}
	tmp2[i] = '\0';
	*k = j;
	return (tmp2);
}

char	*copy_between(char *str)
{
	char	*tmp;
	int		i;

	i = 0;
	while (str[i] != '$')
		i++;
	if (i == 0)
		return (NULL);
	tmp = malloc(sizeof(char *) * i + 1);
	i = 0;
	while (str[i] != '$')
	{
		tmp[i] = str[i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}

char	*ret_exp(int i, char *str, t_env *env, char *tmp)
{
	char	*tmp2;
	char	*expanded;

	expanded = NULL;
	tmp2 = copy_from_var(&i, str);
	expanded = expand_var(tmp2, NULL, env);
	if (tmp2)
		free(tmp2);
	if (expanded)
	{
		if (tmp)
		{
			tmp2 = ft_strjoin(tmp, expanded);
			free(tmp);
		}
		else
			tmp2 = expanded;
		if (strchr(&str[i], '$'))
			tmp2 = handle_multiple_expansion(str, i, tmp2, env);
		return (tmp2);
	}
	else
		return (tmp);
}

int	calc_dollar(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '$')
		i++;
	return (i);
}
