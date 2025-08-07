/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:06:03 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/07 07:40:43 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

// if (str && str[0] != '\0' && str[1] == '\0')
// t_token *new_token(token_type type, char *value, int quote, int new_word)
// {
// 	char c;
//     t_token *token = malloc(sizeof(t_token));
//    // if (type == TOKEN_INVALID || value == NULL)
//    // {
// 	//    token->type = TOKEN_INVALID;
// 	 //   token->value = NULL;
// 	 //   token->next = NULL;
// 	 //   return(token);
// //    }
//     token->type = type;
// 	//if (strlen(value) > 1)
// 	//{
// 		if (value[0] == '$' && !value[1] || value[1] =='"' || value[1] == '"')
// 			token->value = ft_strdup_char('$');
// 	//}
// 	else if (type == TOKEN_WORD)
//     	token->value = ft_strdup(value);
// 	else if (type == TOKEN_HEREDOC)
// 		token->value = ft_strdup("<<");
// 	else if (type == TOKEN_APPEND)
// 		token->value = ft_strdup(">>");
// 	else
// 		token->value = ft_strdup_char(value[0]);
// 	//if	(strchr(value, '$'))
	
// 	token->inside_double = 0;
// 	token->inside_single = 0;
// 	if (quote == 1)
// 		token->inside_double = 1;
// 	else if (quote == 2 || type == TOKEN_QUOTE_SINGLE)
// 		token->inside_single = 1;
// 	token->new_word = new_word;
// 	//printf("value[1] = %s\n", value[1]);
// 	if (value[0] == '$' && quote != 2 && (value[1] != '\'' && value[1] != '"'))
// 	{
// 		token->type = TOKEN_VARIABLE;
// 		if (token->value)
// 			free(token->value);
// 		token->value = ft_strdup_char('$');
// 		if (value[1] == '\0')
// 			token->type = TOKEN_WORD;
// 	}
// 	//else
// 	//	token->type = TOKEN_WORD;
//     token->next = NULL;
//     return (token);
// }


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
    if (value[0] == '$' && (!value[1] || value[1] =='"' || value[1] == '"'))
        token->value = ft_strdup_char('$');
    else if (type == TOKEN_WORD)
        token->value = ft_strdup(value);
    else if (type == TOKEN_HEREDOC)
        token->value = ft_strdup("<<");
    else if (type == TOKEN_APPEND)
        token->value = ft_strdup(">>");
    else
        token->value = ft_strdup_char(value[0]);
    //if    (strchr(value, '$'))

    token->inside_double = 0;
    token->inside_single = 0;
    if (quote == 1)
        token->inside_double = 1;
    else if (quote == 2 || type == TOKEN_QUOTE_SINGLE)
        token->inside_single = 1;
    token->new_word = new_word;
    //printf("value[1] = %s\n", value[1]);
    if (value[0] == '$' && quote != 2 && (value[1] != '\'' && value[1] != '"'))
    {
        token->type = TOKEN_VARIABLE;
        if (token->value)
            free(token->value);
        token->value = ft_strdup_char('$');
        if (value[1] == '\0')
            token->type = TOKEN_WORD;
    }
    //else
    //  token->type = TOKEN_WORD;
    token->next = NULL;
   
	if (value && value[1] == '\0' && type == TOKEN_WORD)
	{
  //      printf("value being freed in new_token = %s\n", value);
			 if (value)
			 	free(value);
	//			*value = NULL;
	}
    
    return (token);
}