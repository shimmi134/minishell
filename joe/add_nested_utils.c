/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_nested_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 18:30:09 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/21 20:39:09 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_slash(t_struct_var *structs, t_quote_vars *vars)
{
	if ((*structs->current))
	{
		(*structs->current)->next = new_token(TOKEN_WORD, ft_strdup_char('/'),
				1, vars->new_word);
		*structs->current = (*structs->current)->next;
	}
	else
	{
		*structs->head = new_token(TOKEN_WORD, ft_strdup_char('/'), 1,
				vars->new_word);
		*structs->current = *structs->head;
	}
}

void	add_single(t_struct_var *structs, t_quote_vars *vars)
{
	if ((*structs->current))
	{
		(*structs->current)->next = new_token(TOKEN_WORD, ft_strdup_char('\''),
				1, vars->new_word);
		*structs->current = (*structs->current)->next;
	}
	else
	{
		*structs->head = new_token(TOKEN_WORD, ft_strdup_char('\''), 1,
				vars->new_word);
		*structs->current = *structs->head;
	}
}

void	add_arr(t_quote_vars *vars, t_struct_var *structs, int *i)
{
	t_token	*token;
	t_token	**current;

	current = structs->current;
	token = NULL;
	if (*structs->current)
	{
		if (vars->arr && vars->arr[1])
		{
			(*structs->current)->next = new_token(TOKEN_WORD, vars->arr,
					vars->quote_flag, vars->new_word);
			*structs->current = (*structs->current)->next;
		}
		else if (vars->arr)
		{
			(*structs->current)->next = new_token(TOKEN_WORD,
					ft_strdup(vars->arr), vars->quote_flag, vars->new_word);
			*structs->current = (*structs->current)->next;
		}
		if (vars->arr)
			free_and_null(&vars->arr);
	}
	else
		add_arr_to_head(structs, vars);
}
