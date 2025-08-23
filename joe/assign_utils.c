/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 18:32:21 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/23 18:04:40 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	assign_arr_memory(t_quote_vars *vars)
{
	if (vars->arr)
		free(vars->arr);
	if (vars->k > 0)
	{
		vars->arr = malloc(sizeof(char) * (vars->k + 1));
		if (!vars->arr)
			exit(EXIT_FAILURE);
		vars->arr[vars->k] = '\0';
		vars->j = 0;
		vars->k = 0;
	}
}

void	assign_var_token(t_struct_var *structs, t_quote_vars *vars, char *str,
		int *i)
{
	if ((*structs->current))
	{
		(*structs->current)->next = new_token(TOKEN_WORD, &str[(*i)], 1,
				vars->new_word);
		(*structs->current) = (*structs->current)->next;
	}
	else
	{
		*structs->head = new_token(TOKEN_WORD, &str[(*i)], 1, vars->new_word);
		*structs->current = *structs->head;
	}
	vars->new_word = 0;
}

t_token	*assign_word_arr(char *arr, int new_word)
{
	t_token	*token;

	if (ft_strlen(arr) > 1)
		token = new_token(TOKEN_WORD, arr, 0, new_word);
	else
		token = new_token(TOKEN_WORD, ft_strdup_char(arr[0]), 0, new_word);
	return (token);
}

void	assign_after_var(char *str, t_quote_vars *vars, int *i)
{
	vars->k = 0;
	while (str[vars->j] && str[vars->j] != '"' && str[vars->j] != '\''
		&& str[vars->j] != '$' && str[vars->j] != '/' && str[vars->j] != '?')
	{
		vars->k++;
		vars->j++;
	}
	if (vars->j == (*i) && str[vars->j] != '$' && str[vars->j] != '\''
		&& str[vars->j] != '/' && str[vars->j] != '?')
	{
		if (vars->arr)
			free(vars->arr);
		vars->arr = NULL;
		return ;
	}
}
