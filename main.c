/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:05:40 by joshapir          #+#    #+#             */
/*   Updated: 2025/06/21 19:57:19 by joshapir         ###   ########.fr       */
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
	if (!head)
		return ;
    while (head)
    {
        if (head->cmd)
           free(head->cmd);
		if (head->heredoc_delim)
			free(head->heredoc_delim);
		if (head->infile)
			free(head->infile);
		if(head->outfile)
			free(head->outfile);
		while (head->args[i])
		{
			free(head->args[i++]);
		}

		tmp = head->next;
		if (head)
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

void handle_sigint(int sig_num)
{
    (void)sig_num;
    rl_replace_line("", 0);
    write(1, "\n", 1);
    rl_on_new_line();
    rl_redisplay();
}


//compile with:  cc *.c -L/usr/include -lreadline
int main(int argc, char *argv[], char *envp[])
{
	char *line;
	t_token *node;
	t_token *head;
	t_cmd *t_head;
	t_env	*env;
	pid_t pid;
	env = copy_env(envp);
	while (1)
	{
			head = NULL;
			t_head = NULL;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
            line = readline("\033[1;34mminishell>\033[0m ");
		//  line = strdup("< h")
		//	printf("line = %s\n", line);
		if (line == NULL)
		{
			printf("exit\n");
			break ;
		}
			if (check_quotes(head, line))
			{
				node = lexer(line);
				head = node;
			//	print_list(node);
				if (check_tokens(head))
				{
					t_head = init_cmds(node, env);
				if (ft_strcmp(t_head->cmd, "exit") == 0)
				{
					free_tokens(head);
			 		free_cmds(t_head);
					break ;
				}
				print_cmd_list(t_head);
				t_cmd *hd_temp;
				hd_temp = t_head;
				
				while (hd_temp->next)
					hd_temp = hd_temp->next;
				
		if (hd_temp->heredoc_delim)
		{
			hd_temp->heredoc_fd = read_heredoc(hd_temp->heredoc_delim, hd_temp->heredoc_quoted, env);
			if (hd_temp->heredoc_fd == -1)
    		{
        		  free(line);
            		break ;
    		}
		
				pid = fork();
				if (pid == 0)
				{
					if (hd_temp->heredoc_fd != -1)
				{
    				dup2(hd_temp->heredoc_fd, STDIN_FILENO);
    				close(hd_temp->heredoc_fd);
				}
				if (hd_temp->cmd)
					execvp(hd_temp->cmd, hd_temp->args);
			
					// perror("execvp");
				exit(EXIT_FAILURE);
				}
				else if (pid > 0)
				{
					if (hd_temp->heredoc_fd != -1)
						close(hd_temp->heredoc_fd);
						    int status;
   					 waitpid(pid, &status, 0);
					 free(line);
				}
				else
				{
    				perror("fork");
    				free(line);
				}
				
		}
		}
			
			//printf("head: %s\n", head->value);
			//printf("%s\n", expand_var(head->next, env));
			//  free_tokens(head);
			//  	free_cmds(t_head);
			}
			 free_tokens(head);
			  	free_cmds(t_head);

	}
	return (0);
}