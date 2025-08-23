/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 18:51:10 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/23 16:25:00 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_single(t_quote_vars *vars, int *i, char *str)
{
	if (!vars->arr)
	{
		vars->j = (*i);
		while (str[vars->j] && str[vars->j] != vars->type)
			vars->j++;
		vars->arr = malloc(sizeof(char) * (vars->j + 1));
		if (!vars->arr)
			exit(EXIT_FAILURE);
	}
	if (vars->arr)
	{
		vars->j = 0;
		while ((str[(*i)]) && str[(*i)] != vars->quote)
		{
			vars->arr[vars->j] = str[(*i)];
			(*i)++;
			vars->j++;
		}
		vars->arr[vars->j] = '\0';
	}
	vars->quote_flag = 2;
}

char	quote_type(int type)
{
	int	quote;

	if (type == TOKEN_QUOTE_SINGLE)
		quote = '\'';
	else
		quote = '"';
	return (quote);
}

void	handle_empty_quotes(int *i, int new_word, t_token **head,
		t_token **current)
{
	t_token	*token;

	token = new_token(TOKEN_WORD, ft_strdup_char('\0'), 1, new_word);
	*i += 1;
	if ((*current))
	{
		(*current)->next = token;
		*current = (*current)->next;
	}
	else
	{
		*head = token;
		*current = *head;
	}
}

t_token	*add_word(char *str, int *i)
{
	int		j;
	char	*arr;
	t_token	*current;
	int		new_word;

	j = *i;
	new_word = 0;
	current = NULL;
	if (j > 0 && str[j - 1] == ' ')
		new_word = 1;
	arr = NULL;
	while ((str[j]) && !is_token(str[j]) && str[j] != ' ' && str[j] != '/')
		j++;
	if (j >= (*i))
	{
		allocate_word_arr(&arr, str, i, j);
		current = assign_word_arr(arr, new_word);
		if (strlen(arr) > 1)
			free_and_null(&arr);
	}
	if (arr)
		free(arr);
	return (current);
}

void	quote_if(char *str, t_token **head, t_token **current, int *i)
{
	int				new_word;
	int				type;
	int				quote;
	t_struct_var	*structs;

	structs = malloc(sizeof(t_struct_var));
	if (!structs)
		exit(EXIT_FAILURE);
	structs->head = head;
	structs->current = current;
	type = find_token_type(&str[(*i)]);
	if (type == TOKEN_QUOTE_DOUBLE)
		quote = 1;
	else if (type == TOKEN_QUOTE_SINGLE)
		quote = 2;
	if ((*i) > 0 && str[(*i) - 1] == ' ')
		new_word = 1;
	handle_quote(str, i, type, structs);
	head = structs->head;
	current = structs->current;
	free(structs);
}
