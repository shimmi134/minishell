/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nested_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 18:35:59 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/24 22:56:13 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	flush_arr_in_single(t_struct_var *structs, t_quote_vars *vars)
{
	if (*structs->current)
	{
		if (ft_strlen(vars->arr) == 1)
			(*structs->current)->next = new_token(TOKEN_WORD,
					ft_strdup_char(vars->arr[0]), 1, vars->new_word);
		else
			(*structs->current)->next = new_token(TOKEN_WORD, vars->arr, 1,
					vars->new_word);
		if (vars->arr)
		{
			free(vars->arr);
			vars->arr = NULL;
		}
		*structs->current = (*structs->current)->next;
	}
	else
		flush_to_head(structs, vars);
}

void	flush_to_head(t_struct_var *structs, t_quote_vars *vars)
{
	if (ft_strlen(vars->arr) == 1)
		*structs->head = new_token(TOKEN_WORD, ft_strdup_char(vars->arr[0]), 1,
				vars->new_word);
	else
		*structs->head = new_token(TOKEN_WORD, vars->arr, 1, vars->new_word);
	if (vars->arr)
	{
		free_and_null(&vars->arr);
	}
	*structs->current = *structs->head;
}

void	add_arr_to_head(t_struct_var *structs, t_quote_vars *vars)
{
	if (vars->arr && vars->arr[1])
		(*structs->head) = new_token(TOKEN_WORD, vars->arr, vars->quote_flag,
				vars->new_word);
	else if (vars->arr)
		(*structs->head) = new_token(TOKEN_WORD, ft_strdup(vars->arr),
				vars->quote_flag, vars->new_word);
	if (vars->arr)
		free_and_null(&vars->arr);
	*structs->current = *structs->head;
}

void	handle_slash(t_token **head, t_token **current, char *str, int *i)
{
	int	j;
	int	new_word;

	new_word = 0;
	j = *i;
	if (j > 0 && str[j - 1] == ' ')
		new_word = 1;
	if (*current)
	{
		(*current)->next = new_token(TOKEN_WORD, ft_strdup_char('/'), 0,
				new_word);
		*current = (*current)->next;
	}
	else
	{
		*head = new_token(TOKEN_WORD, ft_strdup_char('/'), 0, new_word);
		*current = *head;
	}
	(*i)++;
}

void	handle_status(t_token **head, t_token **current, char *str, int *i)
{
	int	j;
	int	new_word;

	new_word = 0;
	j = *i;
	if (j > 0 && str[j - 1] == ' ')
		new_word = 1;
	if (*current)
	{
		(*current)->next = new_token(TOKEN_WORD, ft_strdup_char('?'), 0,
				new_word);
		*current = (*current)->next;
	}
	else
	{
		*head = new_token(TOKEN_WORD, ft_strdup_char('?'), 0, new_word);
		*current = *head;
	}
	(*i)++;
}
