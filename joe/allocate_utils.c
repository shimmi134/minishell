/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 18:31:25 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/23 18:18:35 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	allocate_arr(t_quote_vars *vars)
{
	if (vars->arr)
		free(vars->arr);
	vars->arr = malloc(sizeof(char) * (vars->j + 1));
	if (!(vars->arr))
		exit(EXIT_FAILURE);
}

void	allocate_word_arr(char **arr, char *str, int *i, int j)
{
	*arr = malloc(sizeof(char) * (j + 1));
	if (!*arr)
		exit(EXIT_FAILURE);
	j = 0;
	while ((str[(*i)]) && !is_token(str[(*i)]) && str[(*i)] != ' '
		&& str[(*i)] != '/' && str[(*i)] != '\'' && str[(*i)] != '?')
	{
		(*arr)[j++] = str[(*i)++];
	}
	(*arr)[j] = '\0';
}

void	allocate_after_single(char *str, t_quote_vars *vars, int *i)
{
	vars->k = 0;
	while (str[vars->j] && str[vars->j] != '"' && str[(*i)] != '\''
		&& str[vars->j] != '$' && str[vars->j] != '/' && str[vars->j] != '?')
	{
		vars->k++;
		vars->j++;
	}
	if (vars->j == (*i) && str[vars->j] != '$' && str[vars->j] != '\''
		&& str[vars->j] != '/' && str[vars->j] != '?')
	{
		vars->arr = NULL;
		return ;
	}
	assign_arr_memory(vars);
}
