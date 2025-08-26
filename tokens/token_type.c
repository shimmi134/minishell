/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:06:30 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/21 18:10:00 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_token(char c)
{
	if (c == '|')
		return (1);
	else if (c == '$')
		return (1);
	else if (c == '\'')
		return (1);
	else if (c == '\"')
		return (1);
	else if (c == '<')
		return (1);
	else if (c == '>')
		return (1);
	else if (c == '"')
		return (1);
	else
		return (0);
}

int	is_word(char *str)
{
	int	i;
	int	var;

	i = 0;
	var = 0;
	while (str[i])
		i++;
	if (str[i - 1] == '\'' && str[0] == '\'')
		return (1);
	if (str[0] == '$')
		return (1);
	i = 0;
	while (str[i])
	{
		if (is_token(str[i]))
			return (0);
		i++;
	}
	return (1);
}

t_type	find_token_type(char *str)
{
	char	c;

	c = str[0];
	if (c == '|')
		return (TOKEN_PIPE);
	else if (c == '$')
		return (TOKEN_VARIABLE);
	else if (c == '\'')
		return (TOKEN_QUOTE_SINGLE);
	else if (c == '\"')
		return (TOKEN_QUOTE_DOUBLE);
	else if (c == '<' && str[1] == '<')
		return (TOKEN_HEREDOC);
	else if (c == '>' && str[1] == '>')
		return (TOKEN_APPEND);
	else if (c == '<')
		return (TOKEN_REDIRECT_IN);
	else if (c == '>')
		return (TOKEN_REDIRECT_OUT);
	else
		return (TOKEN_INVALID);
}
