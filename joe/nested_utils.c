/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nested_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 18:36:57 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/23 20:16:15 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_nested_var(t_struct_var *structs, t_quote_vars *vars, char *str,
		int *i)
{
	t_token	**current;

	current = structs->current;
	if (vars->first != 0)
	{
		vars->new_word = 0;
		vars->first = 1;
	}
	flush_arr_in_var(vars, structs);
	assign_var_token(structs, vars, str, i);
	if (vars->arr)
	{
		free(vars->arr);
		vars->arr = NULL;
	}
	if (str[(*i) + 1])
		(*i)++;
	vars->j = (*i);
	vars->k = 0;
	assign_after_var(str, vars, i);
	if (vars->k > 0)
		assign_arr_memory(vars);
}

void	handle_nested_single(t_struct_var *structs, t_quote_vars *vars,
		char *str, int *i)
{
	if (vars->j > 0)
	{
		vars->arr[vars->j] = '\0';
		flush_arr_in_single(structs, vars);
		vars->new_word = 0;
	}
	add_single(structs, vars);
	vars->new_word = 0;
	if (str[(*i) + 1])
		(*i)++;
	vars->j = (*i);
	allocate_after_single(str, vars, i);
}

void	flush_arr_in_var(t_quote_vars *vars, t_struct_var *structs)
{
	if (vars->j > 0 && vars->arr)
	{
		if (*structs->current)
		{
			if (ft_strlen(vars->arr) == 1)
			(*structs->current)->next = new_token(TOKEN_WORD,
					ft_strdup_char(vars->arr[0]), 1, vars->new_word);
			else
				(*structs->current)->next = new_token(TOKEN_WORD, vars->arr, 1,
					vars->new_word);
			*structs->current = (*structs->current)->next;
		}
		else
		{
			flush_to_head(structs, vars);
		}
		vars->new_word = 0;
	}
}

void	handle_nested_slash(t_struct_var *structs, t_quote_vars *vars,
		char *str, int *i)
{
	if (vars->j > 0)
	{
		vars->arr[vars->j] = '\0';
		flush_arr_in_single(structs, vars);
		vars->new_word = 0;
	}
	add_slash(structs, vars);
	vars->new_word = 0;
	if (str[(*i) + 1])
		(*i)++;
	vars->j = (*i);
	allocate_after_single(str, vars, i);
}

void	handle_nested_status(t_struct_var *structs, t_quote_vars *vars,
		char *str, int *i)
{
	if (vars->j > 0)
	{
		vars->arr[vars->j] = '\0';
		flush_arr_in_single(structs, vars);
		vars->new_word = 0;
	}
	add_status(structs, vars);
	vars->new_word = 0;
	if (str[(*i) + 1])
		(*i)++;
	vars->j = (*i);
	allocate_after_single(str, vars, i);
}
