/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 18:38:14 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/23 20:41:36 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_quote_vars(char **arr, int *quote, int type, int *i)
{
	*arr = NULL;
	*quote = 0;
	if (type == TOKEN_QUOTE_DOUBLE)
		*quote = 1;
	else if (type == TOKEN_QUOTE_SINGLE)
		*quote = 2;
	return ((*i));
}

void	init_quoted_vars(t_quote_vars *vars)
{
	vars->arr = NULL;
	vars->first = 0;
	vars->j = 0;
	vars->k = 0;
	vars->new_word = 0;
	vars->quote = 0;
	vars->type = 0;
	vars->quote_flag = 0;
}

void	free_quoted_vars(t_quote_vars **vars)
{
	if (!*vars)
		return ;
	if ((*vars)->arr)
	{
		free((*vars)->arr);
		(*vars)->arr = NULL;
	}
	(*vars)->first = 0;
	(*vars)->j = 0;
	(*vars)->k = 0;
	(*vars)->new_word = 0;
	(*vars)->quote = 0;
	(*vars)->type = 0;
	free(*vars);
}

void	free_and_null(char **str)
{
	if (*str)
		free(*str);
	*str = NULL;
}

int	skip(char *str, int i)
{
	int	len;

	if (!str)
		return (i);
	len = ft_strlen(str);
	if (i >= len)
		return (i);
	while (i < len && str[i] == ' ')
		i++;
	return (i);
}
