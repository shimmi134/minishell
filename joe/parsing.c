/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:06:13 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/24 22:22:52 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '"')
		{
			i++;
			while (str[i] && str[i] != '"')
				i++;
			if (str[i] != '"' || i == 0)
				return (printf("Error: no closing quotes\n"), 0);
		}
		else if (str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
				i++;
			if (str[i] != '\'' || i == 0)
				return (printf("Error: no closing quotes\n"), 0);
		}
		i++;
	}
	return (1);
}

int	pipe_if(t_token *tokens)
{
	if (tokens->type == TOKEN_PIPE)
	{
		if (!tokens->next || tokens->next->value[0] == '\0')
			return (printf("Syntax error: pipe without command\n"), 0);
		if ((tokens->next->type != TOKEN_WORD
				&& tokens->next->type != TOKEN_VARIABLE)
			&& (tokens->next->type != TOKEN_REDIRECT_IN
				&& tokens->next->type != TOKEN_REDIRECT_OUT))
			return (printf("Syntax error: pipe without command\n"), 0);
	}
	return (1);
}

int	token_loop_if(t_token *tokens)
{
	if (!pipe_if(tokens))
		return (0);
	else if (tokens->type == TOKEN_HEREDOC || tokens->type == TOKEN_APPEND)
	{
		if (!tokens->next || (tokens->next->type != TOKEN_WORD
				&& tokens->next->type != TOKEN_VARIABLE))
			return (printf("Syntax error near unexpected token\n"), 0);
	}
	else if (tokens->type == TOKEN_REDIRECT_IN
		|| tokens->type == TOKEN_REDIRECT_OUT)
	{
		if (!tokens->next)
			return (printf("Syntax error: expected filename\n"), 0);
		else if ((tokens->next) && tokens->next->type != TOKEN_WORD)
			return (printf("Syntax error: expected filename\n"), 0);
		if ((tokens->next->next && !is_token(tokens->next->next->value[0])
				&& tokens->next->next->type != TOKEN_WORD))
			return ((printf("Syntax error: unexpected token `%s'\n",
						tokens->next->next->value), 0));
	}
	return (1);
}

int	token_loop(t_token *tokens)
{
	while (tokens)
	{
		if (!token_loop_if(tokens))
			return (0);
		tokens = tokens->next;
	}
	return (1);
}

int	check_tokens(t_token *tokens)
{
	t_token	*head;

	head = tokens;
	if (!tokens)
		return (0);
	if (tokens->type == TOKEN_PIPE)
		return (printf("Syntax error, invalid token at start\n"), 0);
	if (tokens->type == TOKEN_VARIABLE && !tokens->next)
		return (printf("Syntax error, invalid token at start\n"), 0);
	if (!token_loop(tokens))
		return (0);
	return (1);
}
