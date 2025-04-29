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
	
	arr = NULL;
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
	return(arr);
}
t_token	*handle_quote(char *str, int *i)
{
	char *arr;
	char c;
	int type;
	t_token *current;
	int flag;
	int j;

	arr = NULL;
	flag = 0;
	j = 0;
	c = str[*i];
	type = find_token_type(&str[*i]);
	if (type == TOKEN_QUOTE_DOUBLE)
			flag = 1;
	if (!str[*i])
	{
		printf("Error: no closing quote\n");
		exit(EXIT_FAILURE);
	}	
	j = *i;
	arr = add_quoted_word(str, &j , type);
	current = new_token(TOKEN_WORD, arr, 1);
	free(arr);
	(*i) = j;
	return(current);
}

t_token *add_word(char *str, int *i)
{
	int j;
	char *arr;
	t_token *current;

	j = *i;
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
	current = new_token(TOKEN_WORD, arr, 0);
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

	i = 0;
	j = 0;
	head = NULL;
	printf("str = %s\n", str);
	while (str[i])
	{
		i = skip(str, i);
		if (is_token(str[i]))
		{
			type = find_token_type(&str[i]);
			if (type == TOKEN_QUOTE_SINGLE || type == TOKEN_QUOTE_DOUBLE)
			{
				if (!head)
				{
					head = handle_quote(str, &i);
					current = head;
				}
				else
				{
					current->next = handle_quote(str, &i);
					current = current->next;
				}
			}
			else
			{
				if (!head)
				{
					head = new_token(type, &str[i], 0);
					current = head;
				}
				else
				{
					current->next = new_token(type, &str[i], 0);
					current = current->next;
				}
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
