/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 18:51:55 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/25 20:53:44 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*handle_no_quote(char *str, t_token *head, t_token **current, int *i)
{
	int	new_word;
	int	type;

	new_word = 0;
	type = find_token_type(&str[(*i)]);
	if ((*i) > 1 && str[(*i) - 1] == ' ')
		new_word = 1;
	if (!head)
	{
		head = new_token(type, &str[(*i)], 0, new_word);
		*current = head;
	}
	else
	{
		(*current)->next = new_token(type, &str[(*i)], 0, new_word);
		*current = (*current)->next;
	}
	if (type == TOKEN_HEREDOC || type == TOKEN_APPEND)
		(*i)++;
	return (head);
}

void	allocate_in_single(t_quote_vars *vars, char *str, int *i)
{
	vars->j = 0;
	while ((str[(*i)]) && str[(*i)] != '\'')
	{
		vars->arr[vars->j] = str[(*i)];
		(*i)++;
		vars->j++;
	}
	vars->arr[vars->j] = '\0';
}
