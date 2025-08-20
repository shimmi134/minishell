/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:05:31 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/20 21:22:08 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int skip(char *str, int i)
{
	int len;

	if (!str)
		return (i);

	len = ft_strlen(str);
	if (i >= len)
		return (i);

	while (i < len && str[i] == ' ')
		i++;

	return (i);
}

/*
void add_token(t_token *lst, char c)
{
	token_type type;

}
	*/

void init_quoted_vars(t_quote_vars *vars)
{
	vars->arr = NULL;
	vars->first = 0;
	vars->j = 0;
	vars->k = 0;
	vars->new_word = 0;
	vars->quote = 0;
	vars->type = 0;
	vars->quote_flag = 0;
}

void free_quoted_vars(t_quote_vars **vars)
{
	if (!*vars)
		return;
	if ((*vars)->arr)
	{
		free((*vars)->arr);
		(*vars)->arr = NULL;
	}
	(*vars)->first = 0;
	(*vars)->j = 0;
	(*vars)->k = 0;
	(*vars)->new_word = 0;
	(*vars)->quote = 0;
	(*vars)->type = 0;
	free(*vars);
}

// char *add_quoted_word(char *str, int *i, int type, t_token **current)
// {
// 	int j;
// 	int k;
// 	char quote;
// 	char *arr;
// 	int new_word;
// 	int first;

// 	arr = NULL;
// 	new_word = 0;
// 	first = 0;
// 	k = 0;
// 	j = 0;
// 	(*i)++;
// 	j = (*i);
// 	if (type == TOKEN_QUOTE_SINGLE)
// 		quote = '\'';
// 	else
// 		quote = '"';
// 	while (str[j] && (str[j] != '\'' && str[j] != '"' && str[j] != '$' && str[j] != '/'))
// 		j++;
// 	if (j > (*i))
// 	{

// 		arr = malloc(sizeof(char) * (j + 1));
// 		if (!arr)
// 			exit(EXIT_FAILURE);
// 		//		printf("address after mallloc = %p\n", arr);
// 	}
// 	else
// 		arr = NULL;
// 	j = 0;
// 	if ((*i) > 1 && str[(*i) - 2] == ' ')
// 		new_word = 1;
// 	if (type == 8)
// 	{
// 		while (str[(*i)] && str[(*i)] != '"')
// 		{
// 			if (str[(*i)] == '\'')
// 			{
// 				if (j > 0)
// 				{
// 					arr[j] = '\0';
// 					if ((*current))
// 					{
// 						if (ft_strlen(arr) == 1)
// 							(*current)->next = new_token(TOKEN_WORD, ft_strdup_char(arr[0]), 1, new_word);
// 						else
// 							(*current)->next = new_token(TOKEN_WORD, arr, 1, new_word);

// 						if (arr && arr[1])
// 							free(arr);

// 						*current = (*current)->next;
// 					}
// 					new_word = 0;
// 				}
// 				if ((*current))
// 				{
// 					(*current)->next = new_token(TOKEN_WORD, ft_strdup_char('\''), 1, new_word);
// 					*current = (*current)->next;
// 				}
// 				new_word = 0;
// 				// if (arr && arr[1])
// 				// 	free(arr);
// 				if (str[(*i) + 1])
// 					(*i)++;
// 				j = (*i);

// 				while (str[j] && str[j] != '"' && str[(*i)] != '\'' && str[j] != '$' && str[j] != '/') // new
// 				{
// 					k++;
// 					j++;
// 				}
// 				if (j == (*i) && str[j] != '$' && str[j] != '\'' && str[j] != '/') // should work
// 					return (NULL);
// 				if (arr)
// 					free(arr);
// 				arr = malloc(sizeof(char) * (k + 1));
// 				if (!arr)
// 					exit(EXIT_FAILURE);
// 				arr[k] = '\0';
// 				j = 0;
// 				k = 0;
// 			}
// 			if (str[(*i)] == '$')
// 			{
// 				if (first != 0)
// 				{
// 					new_word = 0;
// 					first = 1;
// 				}
// 				if (j > 0)
// 				{
// 					if ((*current))
// 					{
// 						(*current)->next = new_token(TOKEN_WORD, arr, 1, new_word);
// 						arr = NULL;
// 						(*current) = (*current)->next;
// 					}
// 					new_word = 0;
// 				}
// 				if (*current)
// 				{
// 					(*current)->next = new_token(TOKEN_WORD, &str[(*i)], 1, new_word);
// 					(*current) = (*current)->next;
// 				}
// 				new_word = 0;
// 				if (arr)
// 					free(arr);
// 				if (str[(*i) + 1])
// 					(*i)++;
// 				j = (*i);
// 				while (str[j] && str[j] != '"' && str[(*i)] != '\'' && str[j] != '$' && str[(*i)] != '/') // new
// 				{
// 					k++;
// 					j++;
// 				}

// 				if (j == (*i) && str[j] != '$' && str[j] != '\'' && str[(*i)] != '/') // should work
// 					return (NULL);
// 				arr = malloc(sizeof(char) * (k + 1));
// 				arr[k] = '\0';
// 				j = 0;
// 				k = 0;
// 			}
// 			j = 0;
// 			//j = (*i);
// 			while (str[(*i)] && str[(*i)] != '"' && str[(*i)] != '\'' && str[(*i)] != '$' && str[(*i)] != '/')
// 			{
// 				arr[j] = str[(*i)];
// 				j++;
// 				(*i)++;
// 			}
// 			//	printf("val of j = %d\n", j);
// 			if (arr)
// 			{
// 				//		printf("goes here\n");
// 				arr[j] = '\0';
// 			}
// 			//		printf("arr to be freed = %s\n", arr);
// 		}
// 	}
// 	else
// 	{
// 		if (!arr)
// 		{
// 			j = (*i);
// 			while (str[j] && str[j] != type)
// 				j++;
// 			arr = malloc(sizeof(char) * (j + 1));
// 		}
// 		if (arr)
// 		{
// 			j = 0;
// 			while ((str[(*i)]) && str[(*i)] != quote)
// 			{
// 				arr[j] = str[(*i)];
// 				(*i)++;
// 				j++;
// 			}
// 			arr[j] = '\0';
// 		}
// 	}
// 	return (arr);
// }

void free_and_null(char **str)
{
	if (*str)
		free(*str);
	*str = NULL;
}

void flush_to_head(t_struct_var *structs, t_quote_vars *vars)
{
	if (ft_strlen(vars->arr) == 1)
		*structs->head = new_token(TOKEN_WORD, ft_strdup_char(vars->arr[0]), 1, vars->new_word);
	else
		*structs->head = new_token(TOKEN_WORD, vars->arr, 1, vars->new_word);
	if (vars->arr)
	{
		free_and_null(&vars->arr);
	}
	*structs->current = *structs->head;
}
void flush_arr_in_single(t_struct_var *structs, t_quote_vars *vars, char *str, int *i)
{
	if (*structs->current)
	{
			if (ft_strlen(vars->arr) == 1)
				(*structs->current)->next = new_token(TOKEN_WORD, ft_strdup_char(vars->arr[0]), 1, vars->new_word);
			else
				(*structs->current)->next = new_token(TOKEN_WORD, vars->arr, 1, vars->new_word);

			if (vars->arr)
			{
				free(vars->arr);
				vars->arr = NULL;
			}
			*structs->current = (*structs->current)->next;
	}
	else
		flush_to_head(structs, vars);

}

void add_slash(t_struct_var *structs, t_quote_vars *vars)
{
	if ((*structs->current))
	{
		(*structs->current)->next = new_token(TOKEN_WORD, ft_strdup_char('/'), 1, vars->new_word);
		*structs->current = (*structs->current)->next;
	}
	else
		{
			*structs->head = new_token(TOKEN_WORD, ft_strdup_char('/'), 1, vars->new_word);
			*structs->current = *structs->head;
		}
}
void add_single(t_struct_var *structs, t_quote_vars *vars)
{
	if ((*structs->current))
	{
		(*structs->current)->next = new_token(TOKEN_WORD, ft_strdup_char('\''), 1, vars->new_word);
		*structs->current = (*structs->current)->next;
	}
	else
		{
			*structs->head = new_token(TOKEN_WORD, ft_strdup_char('\''), 1, vars->new_word);
			*structs->current = *structs->head;
		}
}
void assign_arr_memory(t_quote_vars *vars)
{
	if (vars->arr)
		free(vars->arr);
	if (vars->k > 0)
	{
		vars->arr = malloc(sizeof(char) * (vars->k + 1));
		if (!vars->arr)
			exit(EXIT_FAILURE);
		vars->arr[vars->k] = '\0';
		vars->j = 0;
		vars->k = 0;
	}
}
void	allocate_after_single(char *str, t_quote_vars *vars, int *i)
{
	while (str[vars->j] && str[vars->j] != '"' && str[(*i)] != '\'' && str[vars->j] != '$' && str[vars->j] != '/') // new
	{
		vars->k++;
		vars->j++;
	}
	if (vars->j == (*i) && str[vars->j] != '$' && str[vars->j] != '\'' && str[vars->j] != '/') // should work
	{
		vars->arr = NULL;
		return;
	}
	assign_arr_memory(vars);
	//fill_arr(str, i, vars);
}
// echo "test'test'$USER'test'test$USER'"
void handle_nested_single(t_struct_var *structs, t_quote_vars *vars, char *str, int *i)
{
	if (vars->j > 0)
	{
		vars->arr[vars->j] = '\0';
		flush_arr_in_single(structs, vars, str, i);
		vars->new_word = 0;
	}
	add_single(structs, vars);
	vars->new_word = 0;
	if (str[(*i) + 1])
		(*i)++;
	vars->j = (*i);
	
	allocate_after_single(str, vars, i);
}
void handle_nested_slash(t_struct_var *structs, t_quote_vars *vars, char *str, int *i)
{
	if (vars->j > 0)
	{
		vars->arr[vars->j] = '\0';
		flush_arr_in_single(structs, vars, str, i);
		vars->new_word = 0;
	}
	add_slash(structs, vars);
	vars->new_word = 0;
	if (str[(*i) + 1])
		(*i)++;
	vars->j = (*i);

	allocate_after_single(str, vars, i);
}
void flush_arr_in_var(t_quote_vars *vars, t_struct_var *structs)
{
	if (vars->j > 0 && vars->arr)
	{
		
		if (*structs->current)
		{
			(*structs->current)->next = new_token(TOKEN_WORD, vars->arr, 1, vars->new_word);
			//vars->arr = NULL;
			*structs->current = (*structs->current)->next;
		}
		else
		{
			*structs->head = new_token(TOKEN_WORD, vars->arr, 1, vars->new_word);
		//	vars->arr = NULL;
			*structs->current = *structs->head;
		}
		vars->new_word = 0;
	}
}

void	assign_var_token(t_struct_var *structs, t_quote_vars *vars, char *str, int *i)
{
	if ((*structs->current))
	{
		(*structs->current)->next = new_token(TOKEN_WORD, &str[(*i)], 1, vars->new_word);
		(*structs->current) = (*structs->current)->next;
	}
	else
	{
		*structs->head = new_token(TOKEN_WORD, &str[(*i)], 1, vars->new_word);
		*structs->current = *structs->head;
	}	
	vars->new_word = 0;
}

void	assign_after_var(char *str, t_quote_vars *vars, int *i)
{
	while (str[vars->j] && str[vars->j] != '"' && str[(*i)] != '\'' && str[vars->j] != '$' && str[vars->j] != '/') // new
	{
		vars->k++;
		vars->j++;
	}
	if (vars->j == (*i) && str[vars->j] != '$' && str[vars->j] != '\'' && str[vars->j] != '/') // should work
	{
		if (vars->arr)
			free(vars->arr);
		vars->arr = NULL;
		return;
	}
}
void handle_nested_var(t_struct_var *structs, t_quote_vars *vars, char *str, int *i)
{
	t_token **current;

	current = structs->current;
	if (vars->first != 0)
	{
		vars->new_word = 0;
		vars->first = 1;
	}
	flush_arr_in_var(vars, structs);
	assign_var_token(structs, vars, str, i);
	if (vars->arr)
	{
		free(vars->arr);
		vars->arr = NULL;
	}
	if (str[(*i) + 1])
		(*i)++;
	vars->j = (*i);
	vars->k = 0;
	assign_after_var(str, vars, i);
	if (vars-> k > 0)
		assign_arr_memory(vars);
}
void fill_arr(char *str, int *i, t_quote_vars *vars)
{
	while (str[(*i)] && str[(*i)] != '"' && str[(*i)] != '\'' && str[(*i)] != '$' && str[(*i)] != '/')
	{
		vars->arr[vars->j] = str[(*i)];
		vars->j++;
		(*i)++;
	}
}

char *handle_single(t_quote_vars *vars, int *i, char *str)
{
	if (!vars->arr)
	{
		vars->j = (*i);
		while (str[vars->j] && str[vars->j] != vars->type)
			vars->j++;
		vars->arr = malloc(sizeof(char) * (vars->j + 1));
		if (!vars->arr)
			exit(EXIT_FAILURE);
	}
	if (vars->arr)
	{
		vars->j = 0;
		while ((str[(*i)]) && str[(*i)] != vars->quote)
		{
			vars->arr[vars->j] = str[(*i)];
			(*i)++;
			vars->j++;
		}
		vars->arr[vars->j] = '\0';
	}
	if (vars->arr)
		free(vars->arr);
	vars->quote_flag = 2;
	return (vars->arr);
}
char quote_type(int type)
{
	int quote;
	if (type == TOKEN_QUOTE_SINGLE)
		quote = '\'';
	else
		quote = '"';
	return (quote);
}

void allocate_arr(t_quote_vars *vars)
{
	if (vars->arr)
		free(vars->arr);//new
	vars->arr = malloc(sizeof(char) * (vars->j + 1));
	if (!(vars->arr))
		exit(EXIT_FAILURE);
}

void handle_double(t_quote_vars *vars, int *i, char *str, t_struct_var *structs)
{
	t_token **current;

	current = structs->current;
	while (str[(*i)] && str[(*i)] != '"')
	{
		// if (vars->arr)
		// 	add_arr(vars, current, i);
		if (str[(*i)] == '\'')
		{
			handle_nested_single(structs, vars, str, i);
		}
		else if (str[(*i)] == '$')
			handle_nested_var(structs, vars, str, i);
		else if  (str[(*i)] == '/')
			handle_nested_slash(structs, vars, str, i);
		vars->j = 0;
		fill_arr(str, i, vars);
		if (vars->arr)
			vars->arr[vars->j] = '\0';
	}
	vars->quote_flag = 1;
}

void add_arr_to_head(t_struct_var *structs, t_quote_vars *vars)
{
	if (vars->arr && vars->arr[1])
		(*structs->head) = new_token(TOKEN_WORD, vars->arr, vars->quote_flag, vars->new_word);
	else if (vars->arr)
		(*structs->head) = new_token(TOKEN_WORD, ft_strdup(vars->arr), vars->quote_flag, vars->new_word);
	if (vars->arr)
		free_and_null(&vars->arr);
	*structs->current = *structs->head;
}

void add_arr(t_quote_vars *vars, t_struct_var *structs, int *i)
{
	t_token *token;
	t_token **current;

	current = structs->current;
	token = NULL;
	if (*structs->current)
	{
		if (vars->arr && vars->arr[1])
		{
			(*structs->current)->next = new_token(TOKEN_WORD, vars->arr, vars->quote_flag, vars->new_word);
			*structs->current = (*structs->current)->next;
		}
		else if (vars->arr)
		{
			(*structs->current)->next = new_token(TOKEN_WORD, ft_strdup(vars->arr), vars->quote_flag, vars->new_word);
			*structs->current = (*structs->current)->next;
		}
		if (vars->arr)
			free_and_null(&vars->arr);
	}
	else
		add_arr_to_head(structs, vars);
}
void add_quoted_word_2(char *str, int *i, int type, t_struct_var *structs)
{
	t_quote_vars *vars;
	t_token **current;

	current = structs->current;
	vars = malloc(sizeof(t_quote_vars));
	if (!vars)
		exit(EXIT_FAILURE);
	init_quoted_vars(vars);
	(*i)++;
	vars->j = (*i);
	vars->quote = quote_type(type);
	while (str[vars->j] && (str[vars->j] != '\'' && str[vars->j] != '"' && str[vars->j] != '$' && str[vars->j] != '/' ))
		vars->j++;
	if (vars->j > (*i))
		allocate_arr(vars);
	vars->j = 0;
	if ((*i) > 1 && str[(*i) - 2] == ' ')
		vars->new_word = 1;
	if (type == 8)
		handle_double(vars, i, str, structs);
	else
		vars->arr = handle_single(vars, i, str);
	if (vars->arr)
		add_arr(vars, structs, i);
	free_quoted_vars(&vars);
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

int ft_isascii(int c)
{
	return (c >= 0 && c <= 127);
}
// int assign_concat_flag(char *str, int i, t_token **current)
// {
// 	int k;
// 	int len;
// 	int new_word;
// 	char quote;
// 	if (!str[i])
// 		return (0);
// 	new_word = 0;
// 	if (!*current)
// 		return (0);
// 	k = ft_strlen((*current)->value);
// 	len = ft_strlen(str);
// 	if (!str || !i || !current || !(*current) || !(*current)->value)
// 		return (0);
// 	if (str[i] == '"' || str[i] == '\'')
// 	{
// 		quote = str[i];
// 		if (i > 0)
// 			i--;
// 		while (i <= len && (k - 1) > 0 && str[i] != (*current)->value[k - 1] && str[i] != quote)
// 			i--;
// 	}
// 	else
// 	{
// 		while (i <= len && str[i] != (*current)->value[k - 1])
// 			i--;
// 	}
// 	if (i > 0)
// 	{
// 		if (str[i + 1] == ' ')
// 			new_word = 1;
// 	}
// 	else
// 		new_word = 0;
// 	return (new_word);
// }
/*
int assign_concat_flag(char *str, int *i, t_token **current)
{
	int k;
	int new_word;
	int len;

	if (!str || !i || !current || !(*current) || !(*current)->value)
		return 0;
	len = ft_strlen(str);
	k = ft_strlen((*current)->value);
	if (k == 0)
		k = 1;
	if (*i >= len)
		*i = len - 1;
	while (*i >= 0 && str[*i] != (*current)->value[k - 1])
		(*i)--;
	if (*i >= 0 && (*i + 1) < len && str[*i + 1] == ' ')
		new_word = 1;
	else
		new_word = 0;

	return new_word;
}
*/

int ft_isalpha(int c)
{
	return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122));
}

void handle_empty_quotes(int *i, int new_word, t_token **head, t_token **current)
{
	t_token *token;

	token = new_token(TOKEN_WORD, ft_strdup_char('\0'), 1, new_word);
	*i += 1;
	if ((*current))
	{
		(*current)->next = token;
		*current = (*current)->next;
	}
	else
		*head = token;
		*current = *head;
	// return (token);
}
void handle_quote(char *str, int *i, int type, t_struct_var *structs)
{
	char *arr;
	char c;
	t_token *token;
	int quote;
	int j;
	int new_word;
	t_token **current;

	current = structs->current;
	//	printf("structs->current at begining of handle_quote = %s\n", (*structs->current)->value);
	new_word = 0;
	j = init_quote_vars(&arr, &quote, type, i);
	j = *i;
	if (j > 0 && str[j - 1] == ' ')
		new_word = 1;
	if (str[j + 1] == str[j])
		return (handle_empty_quotes(i, new_word, structs->head, current));
	add_quoted_word_2(str, i, type, structs);
}

// void	handle_head_quote(char *str, int *i, t_token **head, int type)
// {
// 	char *arr;
// 	char c;
// 	t_token *token;
// 	int quote;
// 	int j;
// 	int new_word;
// 	t_token **current;
// 	current = head;
// //	tmp = head;

// 	new_word = 0;
// 	j = init_quote_vars(&arr, &quote, type, i);
// 	j = *i;
// 	if (j > 0 && str[j - 1] == ' ')
// 		new_word = 1;
// 	if (str[j + 1] == str[j])
// 		return (handle_empty_quotes(i, new_word, current));
// 	add_quoted_word_2(str, i, type, current);
// 	//return (&tmp);
// }

t_token *assign_word_arr(char *arr, int new_word)
{
	t_token *token;
	
	if (ft_strlen(arr) > 1)
		token = new_token(TOKEN_WORD, arr, 0, new_word);
	else
		token = new_token(TOKEN_WORD, ft_strdup_char(arr[0]), 0, new_word);
	return(token);
}

void	allocate_word_arr(char **arr, char *str, int *i, int j)
{
	*arr = malloc(sizeof(char) * (j + 1));
		if (!*arr)
			exit(EXIT_FAILURE);
	j = 0;
	while ((str[(*i)]) && !is_token(str[(*i)]) && str[(*i)] != ' ' && str[(*i)] != '/')
		(*arr)[j++] = str[(*i)++];
	(*arr)[j] = '\0';
}
t_token *add_word(char *str, int *i)
{
	int j;
	char *arr;
	t_token *current;
	int new_word;

	j = *i;
	new_word = 0;
	current = NULL;
	if (j > 0 && str[j - 1] == ' ') //new
			new_word = 1;
	arr = NULL;
	while ((str[j]) && !is_token(str[j]) && str[j] != ' ' && str[j] != '/')
		j++;
	if (j >= (*i))
	{
		allocate_word_arr(&arr, str, i, j);
		current = assign_word_arr(arr, new_word);
	if (strlen(arr) > 1)
		free_and_null(&arr);
	}
	if (arr)
		free(arr);
	return (current);
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

// void handle_head(char *str, t_token **current, int *i, t_token **head)
// {
// 	// t_token *head;
// 	t_token *tmp;
// 	int type;

// 	type = find_token_type(&str[(*i)]);
// 	head = NULL;
// 	tmp = NULL;
// 	*head = new_token(TOKEN_WORD, str, 0, 1);
// 	printf("val of head in handle_head = %s\n", (*head)->value);
// 	// free(head->value);
// 	*current = *head;
// 	handle_quote(str, i, type, current);
// 	// printf("currentt = %s\n", head->value);
// 	while ((*current)->next)
// 		*current = (*current)->next;
// 	if (tmp)
// 	{
// 		(*current)->next = tmp;
// 		if ((*current)->next)
// 			*current = (*current)->next;
// 	}
// 	// tmp = head;
// 	// head = head->next;
// 	// free(tmp->value);
// 	free(tmp);
// 	// return (head);
// }

void quote_if(char *str, t_token **head, t_token **current, int *i)
{
	t_token *tmp;
	int new_word;
	int type;
	int quote;
	t_struct_var *structs;

	structs = malloc(sizeof(t_struct_var));
	if (!structs)
		exit(EXIT_FAILURE);
	structs->head = head;
	structs->current = current;
	type = find_token_type(&str[(*i)]);
	if (type == TOKEN_QUOTE_DOUBLE)
		quote = 1;
	else if (type == TOKEN_QUOTE_SINGLE)
		quote = 2;
	if ((*i) > 0 && str[(*i) - 1] == ' ')
		new_word = 1;
	handle_quote(str, i, type, structs);
	head = structs->head;
	current = structs->current;
	free(structs);
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
void handle_slash(t_token **head, t_token **current, char *str, int *i)
{
	int j;
	int new_word;

	new_word = 0;
	j = *i;
	//printf("current before handle_slash = %s\n", (*current)->value);
	if (j > 0 && str[j - 1] == ' ') //new
			new_word = 1;
	if (*current)
	{
		(*current)->next = new_token(TOKEN_WORD, ft_strdup_char('/'), 1, new_word);
		*current = (*current)->next;
	}
	else
	{
			*head = new_token(TOKEN_WORD, ft_strdup_char('/'), 1, new_word);
			*current = *head;
	}
	(*i)++;
}
void if_not_token(char *str, t_token **head, t_token **current, int *i)
{
	//	printf("address of head in if not token = %p\n", (*head));
 	if (str[(*i)] && str[(*i)] == '/')
	 	handle_slash(head, current, str, i);
	if (!*head)
	{
		//	printf("reaches\n");
		*head = add_word(str, i);
		*current = *head;
	//	printf("current = %s\n", (*current)->value);
	}
	else if (*current)
	{
		(*current)->next = add_word(str, i);
		*current = (*current)->next;
//		printf("current is now =%s\n", (*current)->value);
	}
//	printf("current before slash = %s\n", (*current)->value);
	//  if (str[(*i)] && str[(*i)] == '/')
	//  	handle_slash(head, current, str, i);
	//	return (head);
}

void lexer_loop(char *str, t_token **head, t_token **current, int *i)
{
	int type;
	int quote;
	int len;
	t_token *tmp;

	*head = NULL;
	len = ft_strlen(str);
	while (str[(*i)])
	{
	//	printf("str[i] at start of loop = %c\n", str[(*i)]);
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
		if (!tmp && ((*token)->next->value[0] != '?' && ft_strlen((*token)->value) == 1))
			{
					(*token)->next->new_word = prev_new_word;
				if ((*token)->next->next)
				{
				 if ((*token)->next->next->inside_single || (*token)->next->next->inside_double)
				 	(*token)->next->next->new_word = prev_new_word;
				else
					(*token)->next->next->new_word = 0;
				}
			}
}
void check_flags(t_token **token, t_env *env)
{
	t_token *head;
	char *tmp;
	
	head = *token;
	tmp = NULL;
	while (*token)
	{
		if ((*token)->type == TOKEN_VARIABLE && (*token)->next)
		{
		int	prev_new_word = (*token)->new_word;
			tmp = expand_var((*token)->next->value, NULL, env);
			flag_if(tmp, token, prev_new_word);
			(*token)->next->new_word = prev_new_word;
		}
		*token = (*token)->next;
		if (tmp)
			free(tmp);
	}
	 *token = head;
}

void init_struct_var(t_struct_var *structs)
{
	structs->head = NULL;
	structs->current = NULL;
}

t_token *lexer(char *str, t_env *env)
{
	int i;
	int j;
	char *arr;
	t_token *head;
	t_token *current;
	t_token *tmp;
	int type;
	int empty;
	int quote;
	int new_word;
	// t_struct_var *structs;

	// structs = malloc(sizeof(t_struct_var));

	i = 0;
	j = 0;

	head = NULL;
	current = NULL;
	lexer_loop(str, &head, &current, &i);
	 check_flags(&head, env);
	return (head);
}
