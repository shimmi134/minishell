/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:05:31 by joshapir          #+#    #+#             */
/*   Updated: 2025/06/09 22:00:54 by joshapir         ###   ########.fr       */
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
char *add_quoted_word(char *str, int *i, int type, t_token **current)
{
	int j;
	int k;
	char quote;
	char *arr;
	//t_token *current;
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
	// printf("str[j] before loop = %c\n", str[(*i) - 2]);
	if (str[(*i) - 2] == ' ')
		new_word = 1;
	// printf("new word before entering loop = %d\n", new_word);
		
	if (type == 8)
{
	while (str[(*i)] && str[(*i)] != '"')
	{
		if (str[(*i)] == '\'')
		{
			//if (str[j - 1] != '"')
		//	{
				//new_word = 0;
				// if (first != 0)
				// {
				// 	new_word = 0;
				// 	first = 1;
				// }
		//	}
			
			if (j > 0)
			{
				arr[j] ='\0';
				if ((*current))
				{
					(*current)->next = new_token(TOKEN_WORD, ft_strdup(arr), 1, new_word);
					*current = (*current)->next;
				}
				// else 
				// 	(*current) = new_token(TOKEN_WORD, ft_strdup(arr), 1, new_word);
				new_word = 0;
			}
			if ((*current))
			{
				(*current)->next = new_token(TOKEN_WORD, ft_strdup_char('\''), 1, new_word);
				(*current) = (*current)->next;
			}
			// else
			// 	(*current) = new_token(TOKEN_WORD, ft_strdup_char('\''), 1, new_word);
			new_word = 0;
			if (arr)
				free(arr);
			// if (str[(*i)] != '\'')
			// {
			// 	printf("ERROR: NO CLOSING QUOTEE\n");
			// 	exit(EXIT_FAILURE);
			// }
			if (str[(*i) + 1])
				(*i)++;
			j = (*i);
			
			while (str[j] && str[j] != '"' && str[(*i)] != '\'' && str[j] != '$') //new
			{
				k++;
				j++;
			}
			// printf("str[j] in quoted word loop = %c\n", str[j]);
			 if (j == (*i) && str[j] != '$' && str[j] != '\'') //should work
			 {
				// printf("j is equal to i\n");
			 	return (NULL);
			 }
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
			// printf("value of j at the begining of $ if = %d\n", j);
			if (j > 0)
			{
				//arr[j] ='\0';
				// printf("value of arr in $ if = %s\n", arr);
				if ((*current))
				{
					(*current)->next = new_token(TOKEN_WORD, ft_strdup(arr), 1, 0);
					(*current) = (*current)->next;
				}
					// else 
					// 	(*current) = new_token(TOKEN_WORD, ft_strdup(arr), 1, 0);
				new_word = 0;
				
			}
			if (*current)
			{
				(*current)->next = new_token(TOKEN_WORD, &str[(*i)], 1, new_word);
				(*current) = (*current)->next;
			}
			// else
			// 	(*current) = new_token(TOKEN_WORD, ft_strdup_char('$'), 1, 0);	
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
				// printf("j is equal to i in $ if\n");
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
			// printf("arr = %c\n", arr[j]);
			j++;
			(*i)++;
		}
		if (arr)
			arr[j] = '\0';
		// printf("arr after loop = %s\n", arr);
	}
//	printf("current at end of loop = %s\n", (*current)->value);
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
	// if (str[*i] != quote)
	// {
	// 	printf("Error: no closing quote\n");
	// 	exit(EXIT_FAILURE);
	// }
	// printf("arr at end of add quoted = %s\n", arr);
}
// printf("reaches?\n");
return (arr) ;
}
t_token	*handle_quote(char *str, int *i, int type, t_token **current)
{
	char *arr;
	char c;
//	int type;
	t_token *token;
	int quote;
	int j;
	int new_word;

	arr = NULL;
	new_word = 0;
	quote = 0;
	j = 0;
	c = str[*i];
	//type = find_token_type(&str[*i]);
	if (type == TOKEN_QUOTE_DOUBLE)
			quote = 1;
	else if (type == TOKEN_QUOTE_SINGLE)
			quote = 2;
	// if (!str[*i])
	// {
	// 	printf("Error: no closing quote\n");
	// 	exit(EXIT_FAILURE);
	// }	
	j = *i;
	if (str[j - 1] == ' ')
			new_word = 1;
	if (str[j + 1] == str[j])
	{
		// printf("empty token found\n");
		token = new_token(TOKEN_WORD, ft_strdup_char('\0'), 1, new_word);
		*i += 1;
		return(token);	
	}
		arr = add_quoted_word(str, &j , type, current);
	//	new_word = 0;
	//	if (str[j - 1] == ' ')
	//		new_word = 1;
		// printf("arr after quoted word = %s\n", arr);
	//	printf("testtt\n");
	//	printf("current after quoted word = %s\n", (*current)->value);
	// if ((*current)->next)
	// {
	// 	printf("inside if statemnt");
	// 	while ((*current) && (*current)->next)
	// 		*current = (*current)->next;
	// } //new

	//uncomment if problem
	// printf("str[j] after add quoted = %c\n", str[j]);
	// if (quote == 2)
	// {
	// 	if (str[j] != '\'')
	// 	{
	// 		printf("ERROR: NO CLOSING QUOTE\n");
	// 		exit(EXIT_FAILURE);
	// 	}
	// }
	// else
	// 	if (str[j] != '"')
	// 	{
	// 		printf("ERROR: NO CLOSING QUOTE\n");
	// 		exit(EXIT_FAILURE);
	// 	}
	
	// printf("what about here?\n");
	if (arr)
		token = new_token(TOKEN_WORD, arr, quote, new_word);
	else
		token = NULL;
	if (arr)
		free(arr);
	(*i) = j;
	// printf("reaches end of handle quote\n");
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
			//	printf("type = %d\n", type);
				if (str[i - 1] == ' ')
					new_word = 1;
				// if (str[i + 1] == '$' && type != TOKEN_QUOTE_SINGLE)
				// {
				// 	i++;
				// 	if (!head)
				// {
				// 	head = new_token(type, &str[i], quote, new_word);
				// 	current = head;
				// }
				// else
				// {
				// 	current->next = new_token(type, &str[i], quote, new_word);
				// 	current = current->next;
				// }
				// }
				if (!head)
				{
					head = new_token(TOKEN_WORD, ft_strdup(str), 0, 1);
					// printf("head= %s\n",head-> value);
					current = head;
					tmp = handle_quote(str, &i, type, &current);
				//	printf("tmp after handle_quote in head = %s\n", tmp->value);
					//head = head->next;
					while (current->next)
						current = current->next;
				//	head = tmp;
					
					if (tmp)
					{
						// printf("value of tmp = %s\n", tmp->value);
						current->next = tmp;
						//head = tmp;
					//	current = tmp->next;
					
					if (current->next)
						current = current->next;
					}
					head = head->next;
					// printf("current after handle quote = %s\n", current->value);
				}
				else
				{
					tmp = handle_quote(str, &i, type, &current);
					// printf("current after handle quote = %s\n", current->value);
					if (tmp)
					{
						current->next = tmp;
						current = current->next;
					}
				}
				// printf("here\n");
				#include <stdint.h>

				//t_token *tmp = head;
	// 			if (!head)
	// printf("⚠️ head is NULL — list is empty\n");

// int i = 0;
// while (tmp)
// {
// 	printf("token[%d]: %p, value: %s, next: %p\n", i, (void *)tmp, tmp->value, (void *)tmp->next);
// 	if ((uintptr_t)(tmp->next) < 0x1000) {
// 		printf("⚠️ Invalid next pointer detected!\n");
// 		break;
// 	}
// 	tmp = tmp->next;
// 	i++;
// }

				// printf("str[i] after handle quote = %c\n", str[i]);
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
