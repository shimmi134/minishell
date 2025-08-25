/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:05:31 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/25 20:36:08 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	lexer_loop(char *str, t_token **head, t_token **current, int *i)
{
	int		type;
	int		len;

	*head = NULL;
	len = ft_strlen(str);
	while (str[(*i)])
	{
		(*i) = skip(str, (*i));
		if (is_token(str[(*i)]))
		{
			type = find_token_type(&str[(*i)]);
			if (type == TOKEN_QUOTE_SINGLE || type == TOKEN_QUOTE_DOUBLE)
				quote_if(str, head, current, i);
			else if (type != TOKEN_QUOTE_SINGLE && type != TOKEN_QUOTE_DOUBLE)
				*head = handle_no_quote(str, *head, current, i);
			if (*i + 1 <= len)
				(*i)++;
		}
		(*i) = skip(str, (*i));
		if ((str[(*i)]) && !is_token(str[(*i)]))
			if_not_token(str, head, current, i);
	}
}

void	flag_if(char *tmp, t_token **token, int prev_new_word)
{
	if (!tmp && ((*token)->next->value[0] != '?'
			&& ft_strlen((*token)->value) == 1))
	{
		(*token)->next->new_word = prev_new_word;
		if ((*token)->next->next)
		{
			if ((*token)->next->next->inside_single
				|| (*token)->next->next->inside_double)
				(*token)->next->next->new_word = prev_new_word;
			else
				(*token)->next->next->new_word = 0;
		}
	}
}

void	check_flags(t_token **token, t_env *env)
{
	t_token	*head;
	char	*tmp;
	int		prev_new_word;

	head = *token;
	tmp = NULL;
	while (*token)
	{
		if ((*token)->type == TOKEN_VARIABLE && (*token)->next)
		{
			prev_new_word = (*token)->new_word;
			tmp = expand_var((*token)->next->value, NULL, env);
			flag_if(tmp, token, prev_new_word);
			(*token)->next->new_word = prev_new_word;
			if (tmp)
				free(tmp);
		}
		else if ((*token)->type == TOKEN_VARIABLE && !(*token)->next)
		{
			(*token)->type = TOKEN_WORD;
		}
		*token = (*token)->next;
	}
	*token = head;
}

void	init_struct_var(t_struct_var *structs)
{
	structs->head = NULL;
	structs->current = NULL;
}

t_token	*lexer(char *str, t_env *env)
{
	int		i;
	t_token	*head;
	t_token	*current;

	i = 0;
	head = NULL;
	current = NULL;
	lexer_loop(str, &head, &current, &i);
	check_flags(&head, env);
	if (str)
		free(str);
	return (head);
}
