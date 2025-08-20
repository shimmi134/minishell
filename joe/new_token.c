/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:06:03 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/20 21:35:25 by joshapir         ###   ########.fr       */
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

void    init_token_flags(t_token *token, int new_word, int type, int quote)
{
     token->inside_double = 0;
    token->inside_single = 0;
    if (quote == 1)
        token->inside_double = 1;
    else if (quote == 2 || type == TOKEN_QUOTE_SINGLE)
        token->inside_single = 1;
    token->new_word = new_word;
}

void    handle_var_assign(char *value, t_token *token, int quote)
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
t_token *new_token(token_type type, char *value, int quote, int new_word)
{
    char c;
    t_token *token = malloc(sizeof(t_token));
    
    token->type = type;
    if (value[0] == '$' && (!value[1] || value[1] =='"' || value[1] == '\''))
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
	if ((value && value[1] == '\0' || !value[0]) && type == TOKEN_WORD)
	{
			 if (value)
			 	free(value);
	}
    return (token);
}

// void quote_loop(char *str, t_token **current, int *i, char **arr)
// {
// 	int j;
// 	int new_word;
// 	int k;
	
// 	new_word = 0;
// 	j = 0;
// 	k = 0;

// 	if ((*i) > 1 && str[(*i) - 2] == ' ')
// 			new_word = 1;
// 	while (str[(*i)] && str[(*i)] != '"')
// 	{
// 		if (str[(*i)] == '\'')
// 		{
// 			if (j > 0)
// 			{
// 				*arr[j] ='\0';
// 				if ((*current))
// 				{
// 					if (ft_strlen(*arr) == 1)
// 						(*current)->next = new_token(TOKEN_WORD, ft_strdup_char(*arr[0]), 1, new_word);
// 					else
// 						(*current)->next = new_token(TOKEN_WORD, *arr, 1, new_word);
					
// 					if (*arr && *arr[1])
// 					{
// 						free(*arr);
// 						*arr = NULL;
// 					}
// 					*current = (*current)->next;
// 				}
// 				new_word = 0;
// 			}
// 			if ((*current))
// 			{
// 				(*current)->next = new_token(TOKEN_WORD, ft_strdup_char('\''), 1, new_word);
// 				*current = (*current)->next;
// 			}
// 			new_word = 0;
// 			// if (arr && arr[1])
// 			// 	free(arr);
// 			if (str[(*i) + 1])
// 				(*i)++;
// 			j = (*i);
			
// 			while (str[j] && str[j] != '"' && str[(*i)] != '\'' && str[j] != '$') //new
// 			{
// 				k++;
// 				j++;
// 			}
// 			 if (j == (*i) && str[j] != '$' && str[j] != '\'') //should work
// 			 {
// 				*arr = NULL;
// 			 	return ;
// 			 }
// 				if (*arr)
// 				{
// 					free(*arr);
// 					*arr = NULL;
// 				}
// 			*arr = malloc(sizeof(char) * (k + 1));
// 			*arr [k] = '\0';
// 			j = 0;
// 			k = 0;
// 		}
// 		if (str[(*i)] == '$')
// 		{
// 		//	if (first != 0)
// 	//		{
// 				new_word = 0;
// 	//			first = 1;
// 	//		}
// 			if (j > 0)
// 			{
// 				if ((*current))
// 				{
// 					(*current)->next = new_token(TOKEN_WORD, *arr, 1, new_word);
// 					*arr = NULL;
// 					(*current) = (*current)->next;
// 				}
// 				new_word = 0;	
// 			}
// 			if (*current)
// 			{
// 				(*current)->next = new_token(TOKEN_WORD, &str[(*i)], 1, new_word);
// 				(*current) = (*current)->next;
// 			}
// 			new_word = 0;
// 			if (*arr)
// 				free(*arr);
// 			if (str[(*i) + 1])
// 				(*i)++;
// 			j = (*i);
// 			while (str[j] && str[j] != '"' && str[(*i)] != '\'' && str[j] != '$') //new
// 			{
// 				k++;
// 				j++;
// 			}
			
// 			 if (j == (*i) && str[j] != '$' && str[j] != '\'') //should work
// 			 {
// 				*arr = NULL;
// 			 	return ;
// 			 }
// 			*arr = malloc(sizeof(char) * (k + 1));
// 			*arr [k] = '\0';
// 			j = 0;
// 			k = 0;
// 		}
// 		j = 0;
// 	while (str[(*i)] && str[(*i)] != '"' && str[(*i)] != '\'' && str[(*i)] != '$')
// 		{
// 			*arr[j] = str[(*i)];
// 			j++;
// 			(*i)++;
// 		}
// 		if (*arr)
// 		{
// 			printf("goes here\n");
// 			*arr[j] = '\0';
// 		}
// 		printf("arr to be freed = %s\n", *arr);
// 	}
// //	return (*arr);
// }