/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:05:31 by joshapir          #+#    #+#             */
/*   Updated: 2025/07/04 13:40:39 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int skip (char *str, int i)
{
	while ((str[i]) && (str[i] == ' '))
                i++;
	return (i);
}
char *add_quoted_word(char *str, int *i, int type, t_token **current)
{
	int j;
	int k;
	char quote;
	char *arr;
	int new_word;
	int first;
	
	arr = NULL;
	new_word = 0;
	first = 0;
	k = 0;
	(*i)++;
	j = (*i);
	if (type == TOKEN_QUOTE_SINGLE)
		quote = '\'';
	else
		quote = '"';
	while (str[j] && str[j] != '\'' && str[j] != '"' && str[j] != '$')
				j++;
	arr = malloc(sizeof(char) * (j + 1));
	if (!arr)
		exit(EXIT_FAILURE);
	j = 0;
	if (str[(*i) - 2] == ' ')
		new_word = 1;
	if (type == 8)
{
	while (str[(*i)] && str[(*i)] != '"')
	{
		if (str[(*i)] == '\'')
		{
			if (j > 0)
			{
				arr[j] ='\0';
				if ((*current))
				{
					(*current)->next = new_token(TOKEN_WORD, ft_strdup(arr), 1, new_word);
					*current = (*current)->next;
				}
				new_word = 0;
			}
			if ((*current))
			{
				(*current)->next = new_token(TOKEN_WORD, ft_strdup_char('\''), 1, new_word);
				(*current) = (*current)->next;
			}
			new_word = 0;
			if (arr)
				free(arr);
			if (str[(*i) + 1])
				(*i)++;
			j = (*i);
			
			while (str[j] && str[j] != '"' && str[(*i)] != '\'' && str[j] != '$') //new
			{
				k++;
				j++;
			}
			 if (j == (*i) && str[j] != '$' && str[j] != '\'') //should work
			 	return (NULL);
			arr = malloc(sizeof(char) * (k + 1));
			arr [k] = '\0';
			j = 0;
			k = 0;
		}
		if (str[(*i)] == '$')
		{
			if (first != 0)
			{
				new_word = 0;
				first = 1;
			}
			if (j > 0)
			{
				if ((*current))
				{
					(*current)->next = new_token(TOKEN_WORD, ft_strdup(arr), 1, 0);
					(*current) = (*current)->next;
				}
				new_word = 0;
				
			}
			if (*current)
			{
				(*current)->next = new_token(TOKEN_WORD, &str[(*i)], 1, new_word);
				(*current) = (*current)->next;
			}
			new_word = 0;
			if (arr)
				free(arr);
			if (str[(*i) + 1])
				(*i)++;
			j = (*i);
			
			while (str[j] && str[j] != '"' && str[(*i)] != '\'' && str[j] != '$') //new
			{
				k++;
				j++;
			}
			 if (j == (*i) && str[j] != '$' && str[j] != '\'') //should work
			 {
			 	return (NULL);
			 }
			arr = malloc(sizeof(char) * (k + 1));
			arr [k + 1] = '\0';
			j = 0;
			k = 0;
		}
		j = 0;
		while (str[(*i)] && str[(*i)] != '"' && str[(*i)] != '\'' && str[(*i)] != '$')
		{
			arr[j] = str[(*i)];
			j++;
			(*i)++;
		}
		if (arr)
			arr[j] = '\0';
	}
}
else
{
	while ((str[(*i)]) && str[(*i)] != quote)
	{
			arr[j] = str[(*i)];
			(*i)++;
			j++;
	}
	arr[j] = '\0';
}
return (arr) ;
}
t_token	*handle_quote(char *str, int *i, int type, t_token **current)
{
	char *arr;
	char c;
	t_token *token;
	int quote;
	int j;
	int new_word;

	arr = NULL;
	new_word = 0;
	quote = 0;
	j = 0;
	c = str[*i];
	if (type == TOKEN_QUOTE_DOUBLE)
			quote = 1;
	else if (type == TOKEN_QUOTE_SINGLE)
			quote = 2;
	j = *i;
	if (str[j - 1] == ' ')
			new_word = 1;
	if (str[j + 1] == str[j])
	{
		token = new_token(TOKEN_WORD, ft_strdup_char('\0'), 1, new_word);
		*i += 1;
		return(token);	
	}
		arr = add_quoted_word(str, &j , type, current);
	if (arr)
		token = new_token(TOKEN_WORD, arr, quote, new_word);
	else
		token = NULL;
	if (arr)
		free(arr);
	(*i) = j;
	return(token);
}

t_token *add_word(char *str, int *i)
{
	int j;
	char *arr;
	t_token *current;
	int new_word;

	j = *i;
	new_word = 0;
	if (j > 0)
	{
	if (str[j - 1] == ' ')
		new_word = 1;
	}
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
	t_token *tmp;
	int	 type;
	int empty;
	int quote;
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
				if (type == TOKEN_QUOTE_DOUBLE)
					quote = 1;
				else if (type == TOKEN_QUOTE_SINGLE)
					quote = 2;
				if (str[i - 1] == ' ')
					new_word = 1;
				if (!head)
				{
					head = new_token(TOKEN_WORD, ft_strdup(str), 0, 1);
					current = head;
					tmp = handle_quote(str, &i, type, &current);
					while (current->next)
						current = current->next;
					if (tmp)
					{
						current->next = tmp;
					
					if (current->next)
						current = current->next;
					}
					head = head->next;
				}
				else
				{
					tmp = handle_quote(str, &i, type, &current);
					if (tmp)
					{
						current->next = tmp;
						current = current->next;
					}
				}
			}
			else if (type != TOKEN_QUOTE_SINGLE && type != TOKEN_QUOTE_DOUBLE && !empty)
			{
				if (i > 1 && str[i - 1] == ' ')
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
