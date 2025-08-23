/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens_in_cmd_struct.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 18:35:25 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/23 16:23:36 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_redirect(t_cmd **cmd, t_token **token, int type)
{
	int	token_len;

	token_len = 0;
	if (type == TOKEN_REDIRECT_IN)
	{
		if ((*token)->next)
			*token = (*token)->next;
		(*cmd)->infile = ft_strdup((*token)->value);
	}
	else if (type == TOKEN_REDIRECT_OUT)
	{
		if ((*token)->next)
			*token = (*token)->next;
		if ((*cmd)->outfile)
			free((*cmd)->outfile);
		(*cmd)->outfile = ft_strdup((*token)->value);
	}
//	return (token);
}

void	handle_heredoc(t_cmd **cmd, t_token **token)
{
	(*cmd)->heredoc = 1;
	if ((*token)->next)
		*token = (*token)->next;
	(*cmd)->heredoc_delim = ft_strdup((*token)->value);
	if ((*token)->inside_single || (*token)->inside_double)
		(*cmd)->heredoc_quoted = 1;
	//return (token);
}

void	handle_varible(t_cmd **cmd, t_token **token, t_env *envp)
{
	char	*temp;
	int		i;
	char	*joined;

	i = 0;
	while ((*cmd)->args[i])
		i++;
	if (!(*token)->inside_single && i != 0 && !(*token)->new_word)
	{
		temp = expand_var((*token)->next->value, *cmd, envp);
		if (temp)
		{
			joined = ft_strjoin((*cmd)->args[i - 1], temp);
			free((*cmd)->args[i - 1]);
			(*cmd)->args[i - 1] = joined;
			free(temp);
		}
	}
	else if (!(*token)->inside_single)
		(*cmd)->args[i] = expand_var((*token)->next->value, *cmd, envp);
	else
		(*cmd)->args[i] = ft_strdup((*token)->value);
	i = 0;
}

void	handle_pipes(t_cmd **cmds, t_token **tokens)
{
	if (!(*cmds)->cmd)
	{
		(*cmds)->cmd = (*cmds)->args[0];
		shift_left((*cmds)->args);
	}
	(*cmds)->next = new_cmd_token(*tokens);
	*cmds = (*cmds)->next;
//	return (cmds);
}

void	assign_ctl_tokens(t_token **token, t_cmd **cmd, t_env *envp)
{
	int		type;
	int		i;

	i = 0;
	type = (*token)->type;
	if (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT)
		handle_redirect(cmd, token, type);
	else if (type == TOKEN_HEREDOC)
		handle_heredoc(cmd, token);
	else if (type == TOKEN_APPEND)
	{
		(*cmd)->append = 1;
		if ((*token)->next)
			*token = (*token)->next;
		(*cmd)->outfile = ft_strdup((*token)->value);
	}
	else if (type == TOKEN_VARIABLE)
		handle_varible(cmd, token, envp);
//	return (token);
}
