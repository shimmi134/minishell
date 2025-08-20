/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils10.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:03:16 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/20 21:22:31 by shimi-be         ###   ########.fr       */
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
