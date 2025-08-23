/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 18:47:05 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/23 17:18:44 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_double(t_quote_vars *vars, int *i, char *str,
		t_struct_var *structs)
{
	t_token	**current;

	current = structs->current;
	while (str[(*i)] && str[(*i)] != '"')
	{
		if (str[(*i)] == '\'')
		{
			handle_nested_single(structs, vars, str, i);
		}
		else if (str[(*i)] == '$')
			handle_nested_var(structs, vars, str, i);
		else if (str[(*i)] == '/')
			handle_nested_slash(structs, vars, str, i);
		else if (str[(*i)] == '?')
			handle_nested_status(structs, vars, str, i);
		vars->j = 0;
		fill_arr(str, i, vars);
		if (vars->arr)
			vars->arr[vars->j] = '\0';
	}
	vars->quote_flag = 1;
}

void	fill_arr(char *str, int *i, t_quote_vars *vars)
{
	while (str[(*i)] && str[(*i)] != '"' && str[(*i)] != '\''
		&& str[(*i)] != '$' && str[(*i)] != '/' && str[(*i)] != '?')
	{
		vars->arr[vars->j] = str[(*i)];
		vars->j++;
		(*i)++;
	}
}

void	handle_quote(char *str, int *i, int type, t_struct_var *structs)
{
	char	*arr;
	int		quote;
	int		j;
	int		new_word;

	new_word = 0;
	j = init_quote_vars(&arr, &quote, type, i);
	j = *i;
	if (j > 0 && str[j - 1] == ' ')
		new_word = 1;
	if (str[j + 1] == str[j])
		return (handle_empty_quotes(i, new_word,
				structs->head, structs->current));
	add_quoted_word_2(str, i, type, structs);
}

void	quoted_arr_len(char *str, t_quote_vars *vars)
{
	vars->j = 0;
	while (str[vars->j] && (str[vars->j] != '\'' && str[vars->j] != '"'
			&& str[vars->j] != '$' && str[vars->j] != '/' && str[vars->j] != '?'))
		vars->j++;
}

void	add_quoted_word_2(char *str, int *i, int type, t_struct_var *structs)
{
	t_quote_vars	*vars;
	t_token			**current;

	current = structs->current;
	vars = malloc(sizeof(t_quote_vars));
	if (!vars)
		exit(EXIT_FAILURE);
	init_quoted_vars(vars);
	(*i)++;
	vars->j = (*i);
	vars->quote = quote_type(type);
	quoted_arr_len(str, vars);
	if (vars->j > (*i))
		allocate_arr(vars);
	vars->j = 0;
	if ((*i) > 1 && str[(*i) - 2] == ' ')
		vars->new_word = 1;
	if (type == 8)
		handle_double(vars, i, str, structs);
	else
		handle_single(vars, i, str);
	if (vars->arr)
		add_arr(vars, structs);
	free_quoted_vars(&vars);
}
