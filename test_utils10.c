/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils10.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:03:16 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/24 21:56:55 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_heredoc(t_cmd *t_head, t_env *env, int *exit_status, int *fd_val)
{
	t_heredoc	*heredoc;
	t_shell		*element;
	int			hd_res;

	heredoc = init_heredoc_struct(t_head);
	do_struct(&element, t_head, exit_status);
	hd_res = init_heredoc(heredoc, env, element, fd_val);
	*exit_status = hd_res;
	t_head = NULL;
	env = NULL;
	free_shell(element);
}

int	execute_loop_loop(t_shell *elem, t_env **env, int **last_status_ptr_out)
{
	int	val;

	if (strncmp(elem->type, "built-in", 9) == 0 && elem->next == NULL
		&& strncmp(elem->command->cmd, "exit", 4) != 0)
	{
		val = run_builtin(elem, env);
		*last_status_ptr_out = &val;
		elem = elem->next;
		return (1);
	}
	return (0);
}
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

void	free_combined(int *exit_status, t_env *env)
{
	if (exit_status)
		free(exit_status);
	if (env)
		env = free_env_list_tmp(env);
}

int	pre_exec(char *line, t_env *env, int *exit_status)
{
	t_token	*head;
	t_token	*node;

	head = NULL;
	if (line && *line)
		add_history(line);
	if (line != NULL && check_quotes(line))
	{
		node = lexer(line, env);
		line = NULL;
		head = node;
		//print_list(node);
		if (check_tokens(head))
			if (init_execute(node, head, env, exit_status) == 1)
				return (1);
	}
	if (line)
		free(line);
	if (head)
		free_tokens(head);
	return (0);
}

int	check_line(char *str)
{
	if (str == NULL)
	{
		printf("exit\n");
		return (1);
	}
	return (0);
}
