/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nested_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 22:18:26 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/24 22:18:45 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	(*i)++;
	vars->j = (*i);
	allocate_after_single(str, vars, i);
}

void	handle_nested_space(t_struct_var *structs, t_quote_vars *vars,
		char *str, int *i)
{
	if (vars->j > 0)
	{
		vars->arr[vars->j] = '\0';
		flush_arr_in_single(structs, vars);
		vars->new_word = 0;
	}
	add_space(structs, vars);
	vars->new_word = 0;
	(*i)++;
	vars->j = (*i);
	allocate_after_single(str, vars, i);
}
