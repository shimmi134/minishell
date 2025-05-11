/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:05:31 by joshapir          #+#    #+#             */
/*   Updated: 2025/05/11 16:03:30 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int skip (char *str, int i)
{
	while ((str[i]) && (str[i] == ' '))
                i++;
	return (i);
}
/*
void add_token(t_token *lst, char c)
{
	token_type type;

}
	*/
char *add_quoted_word(char *str, int *i, int type)
{
	int j;
	char quote;
	char *arr;
	t_token *current;
	int new_word;
	
	arr = NULL;
	new_word = 0;
	(*i)++;
	j = (*i);
	if (type == TOKEN_QUOTE_SINGLE)
		quote = '\'';
	else
		quote = '"';
	while ((str[j]) && str[j] != quote)
				j++;
	arr = malloc(sizeof(char) * (j + 1));
	if (!arr)
		exit(EXIT_FAILURE);
	j = 0;
	while ((str[(*i)]) && str[(*i)] != quote)
	{
			arr[j] = str[(*i)];
			(*i)++;
			j++;
	}
	arr[j] = '\0';
	if (str[*i] != quote)
	{
		printf("Error: no closing quote\n");
		exit(EXIT_FAILURE);
	}
	return(arr);
}
t_token	*handle_quote(char *str, int *i)
{
	char *arr;
	char c;
	int type;
	t_token *current;
	int quote;
	int j;
	int new_word;

	arr = NULL;
	new_word = 0;
	quote = 0;
	j = 0;
	c = str[*i];
	type = find_token_type(&str[*i]);
	if (type == TOKEN_QUOTE_DOUBLE)
			quote = 1;
	else if (type == TOKEN_QUOTE_SINGLE)
			quote = 2;
	if (!str[*i])
	{
		printf("Error: no closing quote\n");
		exit(EXIT_FAILURE);
	}	
	j = *i;
	if (str[j - 1] == ' ')
			new_word = 1;
		arr = add_quoted_word(str, &j , type);
	current = new_token(TOKEN_WORD, arr, quote, new_word);
	free(arr);
	(*i) = j;
	return(current);
}

t_token *add_word(char *str, int *i)
{
	int j;
	char *arr;
	t_token *current;
	int new_word;

	j = *i;
	new_word = 0;
	if (str[j - 1] == ' ')
		new_word = 1;
	arr = NULL;
	while ((str[j]) && !is_token(str[j]) && str[j] != ' ')
				j++;
	arr = malloc(sizeof(char) * (j + 1));
	if (!arr)
		exit(EXIT_FAILURE);
	j = 0;
	while ((str[*i]) && !is_token(str[*i]) && str[*i] != ' ')
			arr[j++] = str[(*i)++];
	arr[j] = '\0';
	current = new_token(TOKEN_WORD, arr, 0, new_word);
	free(arr);
	return(current);
}
t_token *lexer (char *str)
{
	int 	i;
	int 	j;
	char	*arr;
	t_token *head;
	t_token *current;
	int	 type;
	int empty;
	int new_word;

	i = 0;
	j = 0;
	
	head = NULL;
	while (str[i])
	{
		new_word = 0;
		empty = 0;
		i = skip(str, i);
		if (is_token(str[i]))
		{
			type = find_token_type(&str[i]);
			if (type == TOKEN_QUOTE_SINGLE || type == TOKEN_QUOTE_DOUBLE)
			{
				if (str[i + 1] == '$')
				{
					i++;
					if (!head)
				{
					head = new_token(type, &str[i], 0, new_word);
					current = head;
				}
				else
				{
					current->next = new_token(type, &str[i], 0, new_word);
					current = current->next;
				}
				}
				if (!head && !empty)
				{
					head = handle_quote(str, &i);
					current = head;
				}
				else if(!empty)
				{
					current->next = handle_quote(str, &i);
					current = current->next;
				}
			}
			else if (type != TOKEN_QUOTE_SINGLE && type != TOKEN_QUOTE_DOUBLE && !empty)
			{
				if (str[i - 1] == ' ')
						new_word = 1;
				if (!head)
				{
					head = new_token(type, &str[i], 0, new_word);
					current = head;
				}
				else
				{
					current->next = new_token(type, &str[i], 0, new_word);
					current = current->next;
				}
				//printf("type = %d\n", type);
				if (type == TOKEN_HEREDOC || type == TOKEN_APPEND)
							i++;
			}
			i++;
		}
		
		i = skip(str, i);
		if((str[i]) && !is_token(str[i]))
		{
				if (!head)
				{
					head = add_word(str, &i);
					current = head;
				}
			else
			{	
				current->next = add_word(str, &i);
				current = current->next;
			}
		}
	}
	return(head);
}
