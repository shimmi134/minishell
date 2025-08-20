/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:05:40 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/20 22:05:20 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <readline/readline.h>

void	print_enum(t_token *list)
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

void	print_list(t_token *head)
{
	t_token	*current;

	current = head;
	while (current != NULL)
	{
		print_enum(current);
		if (current->inside_double)
			printf("[inside double quotes]");
		else if (current->inside_single)
			printf("[inside single quotes]");
		if ((current != head) && !current->new_word)
			printf("[part of prev word]");
		if (current->value && current->value[0] == '\0')
			printf("[empty]\n");
		else
			printf("-> %s\n", current->value);
		current = current->next;
	}
	printf("NULL\n");
}

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		if (head->value)
		{
			free(head->value);
		}
		tmp = head->next;
		free(head);
		head = tmp;
	}
}
void	free_cmds(t_cmd *head)
{
	t_cmd	*tmp;
	int		i;

	i = 0;
	if (!head)
		return ;
	while (head)
	{
		i = 0;
		if (head->cmd)
		{
			free(head->cmd);
		}
		if (head->heredoc_delim)
			free(head->heredoc_delim);
		if (head->infile)
			free(head->infile);
		if (head->outfile)
			free(head->outfile);
		while (head->args[i])
		{
		}
		tmp = head->next;
		if (head->args)
			free(head->args);
		if (head)
			free(head);
		head = tmp;
		i = 0;
	}
}

void	handle_sigint(int sig_num)
{
	(void)sig_num;
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

t_env	*free_env_list_tmp(t_env *env)
{
	t_env	*tmp;
	char	*str;

	if (!env)
		return (NULL);
	while (env)
	{
		tmp = env->next;
		if (env->key)
			free(env->key);
		if (env->value)
		{
			free(env->value);
		}
		free(env);
		env = tmp;
	}
	return (NULL);
}
