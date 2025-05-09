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
	else
		token->value = ft_strdup_char(value[0]);
	token->inside_quotes = quote;
	token->new_word = new_word;
    token->next = NULL;
    return (token);
}
