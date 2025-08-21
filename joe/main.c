/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:05:40 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/21 19:23:01 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <readline/readline.h>

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		if (head->value)
			free(head->value);
		tmp = head->next;
		free(head);
		head = tmp;
	}
}

void	free_if(t_cmd **head, t_cmd **tmp, int i)
{
	if ((*head)->cmd)
		free((*head)->cmd);
	if ((*head)->heredoc_delim)
		free((*head)->heredoc_delim);
	if ((*head)->infile)
		free((*head)->infile);
	if ((*head)->outfile)
		free((*head)->outfile);
	while ((*head)->args[i])
		free((*head)->args[i++]);
	(*tmp) = (*head)->next;
	if ((*head)->args)
		free((*head)->args);
	if ((*head))
		free((*head));
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
		free_if(&head, &tmp, i);
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
