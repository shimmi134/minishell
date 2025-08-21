/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 18:51:55 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/21 18:52:00 by joshapir         ###   ########.fr       */
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
