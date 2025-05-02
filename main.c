#include "minishell.h"
#include <readline/readline.h>

/*
t_token *lexer (char *str)
{
	t_token *head;
	t_token *current;
	int i;
	int j;
	char **arr;
	token_type type;

	i = 0;
	j = 0;
	printf("%s\n", str);
	arr = ft_split(str);
	//head = new_token(TOKEN_INVALID, NULL);
	head = NULL;
	while(arr[i])
	{
			type = find_token_type(arr[i]);
				if (!head)
				{	
					free(head);
					head = new_token(type, arr[i], 0);
					current = head;
				}
				else
				{
					current->next = new_token(type, arr[i], 0);
					current = current->next;
				}
		i++;
	}
	return(head);
}
*/
void print_enum(t_token *list)
{
	if (list->type == 0)
    	printf("TOKEN_WORD ");
    if (list->type == 1)
    	printf("TOKEN_PIPE ");
    if (list->type == 2)
    	printf("TOKEN_REDIRECT_IN ");
    if (list->type == 3)
    	printf("TOKEN_REDIRECT_OUT ");
    if (list->type == 4)
    	printf("TOKEN_APPEND ");
    if (list->type == 5)
    	printf("TOKEN_HEREDOC ");
    if (list->type == 6)
    	printf("TOKEN_SEPARATOR ");
    if (list->type == 7)
    	printf("TOKEN_QUOTE_SINGLE ");
    if (list->type == 8)
    	printf("TOKEN_QUOTE_DOUBLE ");
    if (list->type == 9)
    	printf("TOKEN_VARIABLE ");
    if (list->type == 10)
    	printf("TOKEN_INVALID ");
}


void print_list(t_token *head) 
{
    t_token *current = head;
    while (current != NULL) 
	{
		print_enum(current);
		if (current->inside_quotes)
			printf("[inside quotes]");
        printf("-> %s\n", current->value);
		
        current = current->next;
    }
    printf("NULL\n");
}


void free_tokens(t_token *head)
{
    t_token *tmp;

    while (head)
    {
        if (head->value)
            free(head->value);
		tmp = head->next;
		free(head);
        head = tmp;
    }
}
/*
int main (int argc, char **argv)
{
	int i;

	i = 0;
	(void) argc;
	//char *str = argv[1];
	char *str = "e\"c\"ho""hello";
	//char *str = "test |hello";
	t_token *node = lexer(str);
	t_token *head = node;
	print_list(node);
	free_tokens(head);


}
*/

//compile with:  cc *.c -L/usr/include -lreadline
int main (void)
{
	char *line;
	t_token *node;
	t_token *head;
	while (1)
	{
            line = readline("> ");
			node = lexer(line);
			head = node;
			print_list(node);
			check_tokens(head);
			free_tokens(head);

	}
	return (0);
}