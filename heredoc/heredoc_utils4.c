/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:12:42 by shimi-be          #+#    #+#             */
/*   Updated: 2025/09/04 20:12:46 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	assign_hd_struct_values(t_heredoc **heredoc, t_cmd *cmd)
{
	(*heredoc)->heredoc_delim = ft_strdup(cmd->heredoc_delim[0]);
	(*heredoc)->heredoc_fd = cmd->heredoc_fd;
	(*heredoc)->heredoc_quoted = cmd->heredoc_quoted;
	(*heredoc)->next = NULL;
}

char	*handle_multiple_expansion(char *str, int i, char *tmp2, t_env *env)
{
	char	*tmp3;

	tmp3 = NULL;
	tmp3 = copy_between(&str[i]);
	if (tmp3)
		tmp2 = ft_strjoin(tmp2, tmp3);
	tmp2 = ft_strjoin(tmp2, heredoc_expand(&str[i], 1, env));
	if (tmp3)
		free(tmp3);
	return (tmp2);
}
