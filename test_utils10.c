/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils10.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:03:16 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/21 14:57:38 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_heredoc(t_cmd *t_head, t_env *env, int *exit_status, int *fd_val)
{
	t_heredoc	*heredoc;
	t_shell		*element;
	int			hd_res;
	int			old_stdin;

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
	if (strncmp(elem->type, "built-in", 9) == 0 && elem->next == NULL)
	{
		run_builtin(elem, env);
		*last_status_ptr_out = elem->exit_status_code;
		elem = elem->next;
		return (1);
	}
	return (0);
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
		if (check_tokens(head))
			if (init_execute(node, head, env, exit_status) == 1)
				return (1);
	}
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
