/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens_in_cmd_struct.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 18:35:25 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/24 22:05:43 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void handle_redirect(t_token **token, t_cmd **cmd, int type)
{
	char *arr;
	char *arr2;
	char *tmp;
	char *tmp2;
	
	arr = NULL;
	arr2 = NULL;
	//(*cmd)->append = 1;
		if ((*token)->next)
			*token = (*token)->next;
		if ((*token)->next && !(*token)->next->new_word)
		{
			while (*token)
			{
			
				if ((*token)->next && !(*token)->next->new_word)
				{
					if (!arr)
						tmp = ft_strdup((*token)->value);
					else
						tmp = arr;
					*token = (*token)->next;
					tmp2 = ft_strdup((*token)->value);
					arr = ft_strjoin(tmp, tmp2);
					(*token)->new_word = 1;
				}
				else
					break ;
			}
		}
		else
			arr = ft_strdup((*token)->value);
		if (arr)
		{
			if (type == TOKEN_REDIRECT_IN)
				(*cmd)->infile = ft_strdup(arr);
			else
				(*cmd)->outfile = ft_strdup(arr);
		}
	
}

// void	handle_redirect(t_cmd **cmd, t_token **token, int type)
// {
// 	int	token_len;

// 	token_len = 0;
// 	if (type == TOKEN_REDIRECT_IN)
// 	{
// 		if ((*token)->next)
// 			*token = (*token)->next;
// 		(*cmd)->infile = ft_strdup((*token)->value);
// 	}
// }

void	handle_heredoc(t_cmd **cmd, t_token **token)
{
	(*cmd)->heredoc = 1;
	if ((*token)->next)
		*token = (*token)->next;
	(*cmd)->heredoc_delim = ft_strdup((*token)->value);
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

void	append_while(t_token **token, t_cmd **cmd)
{
	char *arr;
	char *arr2;
	char *tmp;
	char *tmp2;
	
	while (*token)
			{
				if ((*token)->next && !(*token)->next->new_word)
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
					break ;
			}
}
void handle_append(t_token **token, t_cmd **cmd)
{
	char *arr;
	char *arr2;
	// char *tmp;
	// char *tmp2;
	
	arr = NULL;
	arr2 = NULL;
	(*cmd)->append = 1;
		if ((*token)->next)
			*token = (*token)->next;
		if ((*token)->next && !(*token)->next->new_word)
		{
			append_while(token, cmd);
			// while (*token)
			// {
			// 	if ((*token)->next && !(*token)->next->new_word)
			// 	{
			// 		if (!arr)
			// 			tmp = ft_strdup((*token)->value);
			// 		else
			// 			tmp = arr;
			// 		*token = (*token)->next;
			// 		tmp2 = ft_strdup((*token)->value);
			// 		arr = ft_strjoin(tmp, tmp2);
			// 		free(tmp);
			// 		free(tmp2);
			// 		(*token)->new_word = 1;
			// 	}
			// 	else
			// 		break ;
			// }
		}
		else
			arr = ft_strdup((*token)->value);
		if (arr)
		{
			(*cmd)->outfile = ft_strdup(arr);
			free (arr);
		}
}

void add_append(t_token **tokens, t_cmd **cmds)
{
	char *cmd_prev;
	char **args_prev;

	// args_prev = ft_strdup_double((*cmds)->args);
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
