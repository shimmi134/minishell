/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:05:40 by joshapir          #+#    #+#             */
/*   Updated: 2025/05/22 21:19:53 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		if (current->inside_double)
			printf("[inside double quotes]");
		else if (current->inside_single)
			printf("[inside single quotes]");
		if ((current != head) && !current->new_word)
			printf("[part of prev word]");
		if (current->value[0] == '\0')
			printf("[empty]\n");
		else
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
void free_cmds(t_cmd *head)
{
    t_cmd *tmp;
	int i;

	i = 0;
    while (head)
    {
        if (head->cmd)
           free(head->cmd);
//		if (head->heredoc_delim)
//			free(head->heredoc_delim);
//		if (head->infile)
//			free(head->infile);
//		if(head->outfile)
//			free(head->outfile);
		while (head->args[i])
		{
			free(head->args[i++]);
		}
		tmp = head->next;
		free(head);
        head = tmp;
		i = 0;
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
t_env	*create_node(char *env)
{
	char	**split;
	t_env	*node;
	
	split = ft_split(env, '=');
	if (!split)
		return (NULL);
	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = split[0];
	node->value = split[1];
	node->next = NULL;
	return (node);
}

t_env	*copy_env(char *envp[])
{
	
	t_env	*head;
	t_env	*node;
	t_env	*nnode;
	int		i;
	
	head = create_node(envp[0]);
	if (!head)
		return (NULL);
	i = 2;
	node = create_node(envp[1]);
	if (!node)
		return (NULL);
	head->next = node;
	while (envp[i])
	{
		nnode = create_node(envp[i]);
		if (!nnode)
			return (NULL);
		node->next = nnode;
		node = node->next;
		i++;
	}
	return (head);
}

//compile with:  cc *.c -L/usr/include -lreadline
int main(int argc, char *argv[], char *envp[])
{
	char *line;
	t_token *node;
	t_token *head;
	t_cmd *t_head;
	t_env	*env;
	env = copy_env(envp);
	while (1)
	{
			head = NULL;
            line = readline("> ");
			node = lexer(line);
			head = node;
			print_list(node);
			if (check_tokens(head))
				t_head = init_cmds(node, env);
			//printf("head: %s\n", head->value);
			//printf("%s\n", expand_var(head->next, env));
			free_tokens(head);
				free_cmds(t_head);
	}
	return (0);
}
