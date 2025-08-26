/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 22:14:12 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/26 20:08:16 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_redirect(t_token **token, t_cmd **cmd, int type)
{
	char	*arr;
	char	*arr2;

	arr = NULL;
	arr2 = NULL;
	if ((*token)->next)
		*token = (*token)->next;
	if ((*token)->next && !(*token)->next->new_word)
	{
		arr = append_while(token);
	}
	else
		arr = ft_strdup((*token)->value);
	if (arr)
	{
		if (type == TOKEN_REDIRECT_IN)
			(*cmd)->infile = arr;
		else
			(*cmd)->outfile = arr;
	}
}

void	add_append(t_token **tokens, t_cmd **cmds)
{
	char	*cmd_prev;
	char	**args_prev;

	if (!(*cmds)->cmd)
	{
		(*cmds)->cmd = (*cmds)->args[0];
		shift_left((*cmds)->args);
	}
	args_prev = ft_strdup_double((*cmds)->args);
	cmd_prev = ft_strdup((*cmds)->cmd);
	(*cmds)->next = new_cmd_token(*tokens);
	*cmds = (*cmds)->next;
	(*cmds)->append = 1;
	handle_append(tokens, cmds);
	(*cmds)->cmd = cmd_prev;
	if (args_prev)
	{
		free((*cmds)->args);
		(*cmds)->args = args_prev;
	}
}

char	*append_while(t_token **token)
{
	char	*arr;
	char	*tmp;
	char	*tmp2;

	arr = NULL;
	while (*token && (*token)->type == TOKEN_WORD)
	{
		if ((*token)->next && !(*token)->next->new_word
			&& (*token)->next->type == TOKEN_WORD)
		{
			if (!arr)
				tmp = ft_strdup((*token)->value);
			else
				tmp = arr;
			*token = (*token)->next;
			tmp2 = ft_strdup((*token)->value);
			arr = ft_strjoin(tmp, tmp2);
			free(tmp);
			free(tmp2);
			(*token)->new_word = 1;
		}
		else
			return (ft_strdup((*token)->value));
	}
	return (arr);
}
