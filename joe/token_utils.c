/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:06:45 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/24 23:24:05 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	has_token(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (is_token(str[i]))
			return (1);
		i++;
	}
	return (0);
}

void	if_not_token(char *str, t_token **head, t_token **current, int *i)
{
	if (str[(*i)] && str[(*i)] == '/')
		handle_slash(head, current, str, i);
	else if (str[(*i)] && str[(*i)] == '?')
		handle_status(head, current, str, i);
	if (!*head)
	{
		*head = add_word(str, i);
		*current = *head;
	}
	else if (*current)
	{
		(*current)->next = add_word(str, i);
		*current = (*current)->next;
	}
}

t_token	*new_token(t_type type, char *value, int quote, int new_word)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->type = type;
	if (value[0] == '$' && (!value[1] || value[1] == '"' || value[1] == '\''))
		token->value = ft_strdup_char('$');
	else if (type == TOKEN_WORD)
		token->value = ft_strdup(value);
	else if (type == TOKEN_HEREDOC)
		token->value = ft_strdup("<<");
	else if (type == TOKEN_APPEND)
		token->value = ft_strdup(">>");
	else
		token->value = ft_strdup_char(value[0]);
	init_token_flags(token, new_word, type, quote);
	handle_var_assign(value, token, quote);
	token->next = NULL;
	if (value && (value[1] == '\0' || !value[0]) && type == TOKEN_WORD)
	{
		if (value)
			free(value);
	}
	return (token);
}

void	init_token_flags(t_token *token, int new_word, int type, int quote)
{
	token->inside_double = 0;
	token->inside_single = 0;
	if (quote == 1)
		token->inside_double = 1;
	else if (quote == 2 || type == TOKEN_QUOTE_SINGLE)
		token->inside_single = 1;
	token->new_word = new_word;
}

void	handle_var_assign(char *value, t_token *token, int quote)
{
	if (value[0] == '$' && quote != 2 && (value[1] != '\'' && value[1] != '"'))
	{
		token->type = TOKEN_VARIABLE;
		if (token->value)
			free(token->value);
		token->value = ft_strdup_char('$');
		if (value[1] == '\0')
			token->type = TOKEN_WORD;
	}
}
