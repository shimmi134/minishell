/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:05:31 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/02 00:59:43 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


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
	int new_word;
	int first;
	
	arr = NULL;
	new_word = 0;
	first = 0;
	k = 0;
	j = 0;
	(*i)++;
	j = (*i);
	if (type == TOKEN_QUOTE_SINGLE)
		quote = '\'';
	else
		quote = '"';
	while (str[j] && (str[j] != '\'' && str[j] != '"' && str[j] != '$'))
				j++;
	arr = malloc(sizeof(char) * (j + 1));
	if (!arr)
		exit(EXIT_FAILURE);
	j = 0;
	if ((*i) > 1 && str[(*i) - 2] == ' ')
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
					if (ft_strlen(arr) == 1)
						(*current)->next = new_token(TOKEN_WORD, ft_strdup_char(arr[0]), 1, new_word);
					else
						(*current)->next = new_token(TOKEN_WORD, arr, 1, new_word);
					
					if (arr && arr[1])
						free(arr);
					
					*current = (*current)->next;
				}
				new_word = 0;
			}
			if ((*current))
			{
				(*current)->next = new_token(TOKEN_WORD, ft_strdup_char('\''), 1, new_word);
				*current = (*current)->next;
			}
			new_word = 0;
			// if (arr && arr[1])
			// 	free(arr);
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
				if (arr)
					free(arr);
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
					(*current)->next = new_token(TOKEN_WORD, arr, 1, new_word);
					arr = NULL;
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
			 	return (NULL);
			arr = malloc(sizeof(char) * (k + 1));
			arr [k] = '\0';
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
	//	printf("arr to be freed = %s\n", arr);
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

int init_quote_vars(char **arr, int *quote, int type, int *i)
{
	*arr = NULL;
	*quote = 0;
	if (type == TOKEN_QUOTE_DOUBLE)
			*quote = 1;
	else if (type == TOKEN_QUOTE_SINGLE)
			*quote = 2;	
			return ((*i));

}

int	ft_isascii(int c)
{
	return (c >= 0 && c <= 127);
}
int assign_concat_flag(char *str, int j)
{
			j--;
		while (j >= 0 && str[j] && str[j] != '"' && str[j] != '\'' && str[j] != '$')
				j--;
				if (j > 0 && str[j - 1] == ' ')
				return (1);
			else
				return (0);
}
int	ft_isalpha(int c)
{
	return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122));
}

t_token	*handle_quote(char *str, int *i, int type, t_token **current)
{
	char *arr;
	char c;
	t_token *token;
	int quote;
	int j;
	int new_word;

	new_word = 0;
	j = init_quote_vars(&arr, &quote, type, i);
	j = *i;
	if (j > 0 && str[j - 1] == ' ')
			new_word = 1;
	if (str[j + 1] == str[j])
	{
		token = new_token(TOKEN_WORD, ft_strdup_char('\0'), 1, new_word);
		*i += 1;
		return(token);	
	}
		arr = add_quoted_word(str, &j , type, current);
	//new_word = assign_concat_flag(str, j);
			(*i) = j;
		//	if ((*i) > 0)
				//(*i)--;
		// while ((*i) >= 0 && str[(*i)] && str[(*i)] != '"' && str[(*i)] != '\'' && str[(*i)] != '$')
		// 		(*i)--;
		int k;

		k = ft_strlen((*current)->value);
		while (str[(*i)] != (*current)->value[k - 1])
			(*i)--;
	//	printf("str[i] == %c\nstr[k] == %c\n", str[(*i)], str[k]);
	//	printf("current value = %s\nvalue of k = %d\n", (*current)->value, k);
			// if (str[(*i) + 1] == ' ')
			// 	new_word = 1;
			// else
			// new_word = 0;
		if ((*i) > 0 && str[(*i) + 1] == ' ')
			new_word = 1;
		else
			new_word = 0;
	//	}
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
	if (arr && !arr[1])
		current = new_token(TOKEN_WORD, ft_strdup(arr), 0, new_word);
	else
	current = new_token(TOKEN_WORD, arr, 0, new_word);
	 if (arr)
	 	free(arr);
	return(current);
}

// t_token *handle_token(char *str, t_token *head, t_token **current, int *i)
// {
// 	int type;
// 	int quote;
// 	int new_word;
// 	int empty;
// 	t_token *tmp;
	
// 	tmp = NULL;
// 	type = find_token_type(&str[(*i)]);
	
// 			if (type == TOKEN_QUOTE_SINGLE || type == TOKEN_QUOTE_DOUBLE)
// 			{
// 				if (type == TOKEN_QUOTE_DOUBLE)
// 					quote = 1;
// 				else if (type == TOKEN_QUOTE_SINGLE)
// 					quote = 2;
// 				if (str[(*i) - 1] == ' ')
// 					new_word = 1;
// 				if (!head)
// 				{
// 					head = new_token(TOKEN_WORD, ft_strdup(str), 0, 1);
// 					*current = head;
// 					tmp = handle_quote(str, i, type, current);
// 					while ((*current)->next)
// 						*current = (*current)->next;
// 					if (tmp)
// 					{
// 						(*current)->next = tmp;
// 					if ((*current)->next)
// 						*current = (*current)->next;
// 					}
// 					head = head->next;
// 				}
// 				else
// 				{
// 					tmp = handle_quote(str, i, type, current);
// 					if (tmp)
// 					{
// 						(*current)->next = tmp;
// 						*current = (*current)->next;
// 					}
// 					printf("current = %s\n", (*current)->value);
// 				}
// 			}
// 			else if (type != TOKEN_QUOTE_SINGLE && type != TOKEN_QUOTE_DOUBLE && !empty)
// 			{
// 				if ((*i) > 1 && str[(*i) - 1] == ' ')
// 						new_word = 1;
// 				if (!head)
// 				{
// 					head = new_token(type, &str[(*i)], 0, new_word);
// 					*current = head;
// 				}
// 				else
// 				{
// 					(*current)->next = new_token(type, &str[(*i)], 0, new_word);
// 					*current = (*current)->next;
// 				}
// 				if (type == TOKEN_HEREDOC || type == TOKEN_APPEND)
// 							(*i)++;
// 			}
// 			(*i)++;
// 			write(1, "hi\n", 3);
// 	printf("current = %s\n", (*current)->value);
// 		return (head);
// }

t_token *handle_head(char *str, t_token **current, int *i, int type)
{
	t_token *head;
	t_token *tmp;

	head = NULL;
	tmp = NULL;
	head = new_token(TOKEN_WORD, str, 0, 1);
	free(head->value);
	*current = head;
	tmp = handle_quote(str, i, type, current);
	//printf("currentt = %s\n", head->value);
	while ((*current)->next)
		*current = (*current)->next;
	if (tmp)
	{
		(*current)->next = tmp;
		if ((*current)->next)
			*current = (*current)->next;
	}
	tmp = head;
	head = head->next;
	//free(tmp->value);
	free(tmp);
	return (head);
}

t_token *quote_if(char *str, t_token *head, t_token **current, int *i)
{
	t_token *tmp;
	int new_word;
	int type;
	int quote;
	
	type = find_token_type(&str[(*i)]);
				if (type == TOKEN_QUOTE_DOUBLE)
					quote = 1;
				else if (type == TOKEN_QUOTE_SINGLE)
					quote = 2;
				if ((*i) > 0 && str[(*i) - 1] == ' ')
					new_word = 1;
				if (!head)
					head = handle_head(str, current, i, type);
				else
				{
					tmp = handle_quote(str, i, type, current);
					if (tmp)
					{
						(*current)->next = tmp;
						*current = (*current)->next;
					}
				}
		return (head);
}

t_token *handle_no_quote(char *str, t_token *head, t_token **current, int *i)
{
	int new_word;
	int type;

	new_word = 0;
	type = find_token_type(&str[(*i)]);
				if ((*i) > 1 && str[(*i) - 1] == ' ')
						new_word = 1;
				if (!head)
				{
					head = new_token(type, &str[(*i)], 0, new_word);
					*current = head;
				}
				else
				{
					(*current)->next = new_token(type, &str[(*i)], 0, new_word);
					*current = (*current)->next;
				}
				if (type == TOKEN_HEREDOC || type == TOKEN_APPEND)
							(*i)++;
		return (head);
}
t_token *if_not_token(char *str, t_token *head, t_token **current, int *i)
{
				if (!head)
				{
					head = add_word(str, i);
					*current = head;
				}
			else
			{	
				(*current)->next = add_word(str, i);
				*current = (*current)->next;
			}
			return (head);
}
t_token *lexer_loop (char *str, t_token *head, t_token **current, int *i)
{
	int type;
	int quote;
	t_token *tmp;
	
	while (str[(*i)])
	{
		(*i) = skip(str, (*i));
		if (is_token(str[(*i)]))
		{
			type = find_token_type(&str[(*i)]);
			if (type == TOKEN_QUOTE_SINGLE || type == TOKEN_QUOTE_DOUBLE)
				head = quote_if(str, head, current, i);
			else if (type != TOKEN_QUOTE_SINGLE && type != TOKEN_QUOTE_DOUBLE)
				head = handle_no_quote(str, head, current, i);
			(*i)++;
		}
		(*i) = skip(str, (*i));
		if((str[(*i)]) && !is_token(str[(*i)]))
			head = if_not_token(str, head, current, i);
	}
	return (head);
}

t_token *check_flags(t_token *token, t_env *env)
{
	t_token *head;
	char *tmp;
	
	head = token;
	tmp = NULL;
	while (token)
	{
		if (token->type == TOKEN_VARIABLE && token->next)
		{
			tmp = expand_var(token->next->value, NULL, env);
			if (!tmp && token->next->next)
				token->next->next->new_word = 1;
		}
		token = token->next;
	}
	if (tmp)
		free (tmp);
	return (head);
}

t_token *lexer (char *str, t_env *env)
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
	head = lexer_loop(str, head, &current, &i);
	head = check_flags(head, env);
	return(head);
}
