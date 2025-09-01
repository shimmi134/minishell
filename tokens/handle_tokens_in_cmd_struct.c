/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens_in_cmd_struct.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 18:35:25 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/26 20:12:02 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int end_of_delim(t_cmd **cmd)
{
    int i = 0;

    if (!cmd || !*cmd || !(*cmd)->heredoc_delim)
        return 0;  // nothing to count

    while ((*cmd)->heredoc_delim[i])
    {
 //       printf("heredoc_delim[%d] = %s\n", i, (*cmd)->heredoc_delim[i]);
        i++;
    }

//    printf("i at end of loop = %d\n", i);
    return i;
}

void	handle_heredoc(t_cmd **cmd, t_token **token)
{
	int i;

	i = end_of_delim(cmd);
	(*cmd)->heredoc = 1;
	if ((*token)->next)
		*token = (*token)->next;
	(*cmd)->heredoc_delim[i] = ft_strdup((*token)->value);
	if ((*token)->inside_single || (*token)->inside_double)
		(*cmd)->heredoc_quoted = 1;
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
}

void	handle_append(t_token **token, t_cmd **cmd)
{
	char	*arr;
	char	*arr2;

	arr = NULL;
	arr2 = NULL;
	(*cmd)->append = 1;
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
		(*cmd)->outfile = ft_strdup(arr);
		free (arr);
	}
}

void	assign_ctl_tokens(t_token **token, t_cmd **cmd, t_env *envp)
{
	int		type;
	int		i;

	i = 0;
	type = (*token)->type;
	if (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT)
		handle_redirect(token, cmd, type);
	else if (type == TOKEN_HEREDOC)
		handle_heredoc(cmd, token);
	else if (type == TOKEN_APPEND)
	{
		if (!(*cmd)->append)
			handle_append(token, cmd);
		else
		{
			add_append(token, cmd);
		}
	}
	else if (type == TOKEN_VARIABLE)
		handle_varible(cmd, token, envp);
}
