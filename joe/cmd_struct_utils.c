/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_struct_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 18:53:48 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/24 20:38:03 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	shift_left(char **arr)
{
	int		i;

	i = 0;
	while (arr[i + 1])
	{
		arr[i] = arr[i + 1];
		i++;
	}
	arr[i] = NULL;
}

void	handle_join(t_cmd *cmds, int i)
{
	char	*tmp;

	if (!cmds->args[i - 1] || !cmds->args[i])
		return ;
	tmp = ft_strjoin(cmds->args[i - 1], cmds->args[i]);
	free(cmds->args[i - 1]);
	cmds->args[i - 1] = tmp;
	free(cmds->args[i]);
	cmds->args[i] = NULL;
}

void	type_if(int type, t_cmd **cmds, t_token **tokens, t_env *envp)
{
	if (type == TOKEN_PIPE)
		handle_pipes(cmds, tokens);
	else if (type == TOKEN_VARIABLE && !(*tokens)->inside_double
		&& ((*tokens)->next->inside_single || (*tokens)->next->inside_double))
		*tokens = (*tokens)->next;
	if ((*tokens)->type == TOKEN_WORD)
		assign_args(*tokens, cmds);
	else
		assign_ctl_tokens(tokens, cmds, envp);
}

void	cmd_loop(t_token **tokens, t_cmd **cmds, int type, t_env *envp)
{
	int	i;

	while (*tokens)
	{
		i = 0;
		type = (*tokens)->type;
		type_if(type, cmds, tokens, envp);
		while ((*cmds)->args[i])
			i++;
		i = i - 1;
		type = (*tokens)->type;
		if (i > 0 && !(*tokens)->new_word && type != TOKEN_VARIABLE)
			handle_join(*cmds, i);
		if ((*tokens)->type == TOKEN_VARIABLE)
			*tokens = (*tokens)->next;
		*tokens = (*tokens)->next;
	}
}

t_cmd	*init_cmds(t_token *tokens, int exit_code, t_env *env)
{
	t_cmd	*head;
	t_cmd	*cmds;
	int		i;
	int		type;

	i = 0;
	type = 0;
	cmds = new_cmd_token(tokens);
	cmds->exit_code = exit_code;
	head = cmds;
	cmd_loop(&tokens, &cmds, type, env);
	if (!tokens || tokens->new_word)
	{
		while (cmds->next)
			cmds = cmds->next;
		if (!cmds->cmd)
			cmds->cmd = cmds->args[0];
		shift_left(cmds->args);
	}
	return (head);
}
