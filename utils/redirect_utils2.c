/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:21:30 by shimi-be          #+#    #+#             */
/*   Updated: 2025/09/08 16:21:31 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_first_file(t_cmd **cmd, int type)
{
	if (!(*cmd)->infile_first && !(*cmd)->outfile_first)
	{
		if (type == TOKEN_REDIRECT_IN)
			(*cmd)->infile_first = 1;
		else
			(*cmd)->outfile_first = 1;
	}
}
