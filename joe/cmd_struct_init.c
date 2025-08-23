/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_struct_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 18:34:22 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/23 16:11:11 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*new_cmd_token(t_token *tokens)
{
	int		count;
	int		i;
	t_cmd	*cmd;

	i = 0;
	cmd = malloc(sizeof(t_cmd));
	count = arg_count(tokens);
	cmd->args = calloc((count + 1), sizeof(char *));
	if (!cmd->args)
		return (NULL);
	cmd->args[count] = NULL;
	cmd->cmd = NULL;
	cmd->args[0] = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->heredoc_delim = NULL;
	cmd->heredoc_quoted = 0;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->heredoc_fd = -1;
	cmd->exit_status = 0;
	cmd->exit_status2 = 0;
	cmd->exit_code = 0;
	cmd->next = NULL;
	return (cmd);
}

void	assign_args(t_token *tokens, t_cmd **cmds)
{
	int		i;

	i = 0;
	while ((*cmds)->args[i])
		i++;
	if (tokens && tokens->value && tokens->type == TOKEN_WORD)
	{
		(*cmds)->args[i] = ft_strdup(tokens->value);
		i++;
	}
	(*cmds)->args[i] = NULL;
//	return (tokens);
}

int	arg_count(t_token *tokens)
{
	int	count;

	count = 0;
	if (tokens->type == TOKEN_PIPE)
		tokens = tokens->next;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (tokens && tokens->type == TOKEN_WORD)
			count++;
		if (tokens->type == TOKEN_PIPE)
			break ;
		tokens = tokens->next;
	}
	return (count);
}

int	find_char_pos(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (0);
}
