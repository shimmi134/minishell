/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:06:03 by joshapir          #+#    #+#             */
/*   Updated: 2025/05/12 20:13:02 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
void split_token(char *str)
{
	int i;
	char *arr;

	i = 0;

	while(str[i])
	{
		if(is_token(str[i]))
			break ;
		i++;
	}
	

}
*/
t_token *new_token(token_type type, char *value, int quote, int new_word)
{
	char c;
    t_token *token = malloc(sizeof(t_token));
   // if (type == TOKEN_INVALID || value == NULL)
   // {
	//    token->type = TOKEN_INVALID;
	 //   token->value = NULL;
	 //   token->next = NULL;
	 //   return(token);
//    }
    token->type = type;
	if (type == TOKEN_WORD)
    	token->value = ft_strdup(value);
	else if (type == TOKEN_HEREDOC)
		token->value = ft_strdup("<<");
	else if (type == TOKEN_APPEND)
		token->value = ft_strdup(">>");
	else
		token->value = ft_strdup_char(value[0]);
	//if	(strchr(value, '$'))
	if (value[0] == '$')
		token->type = TOKEN_VARIABLE;
	token->inside_double = 0;
	token->inside_single = 0;
	if (quote == 1)
		token->inside_double = 1;
	else if (quote == 2)
		token->inside_single = 1;
	token->new_word = new_word;
    token->next = NULL;
    return (token);
}
