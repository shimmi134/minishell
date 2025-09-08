/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_struct_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:38:22 by shimi-be          #+#    #+#             */
/*   Updated: 2025/09/08 16:38:23 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_redir(t_cmd **cmd)
{
	(*cmd)->infile = NULL;
	(*cmd)->outfile = NULL;
	(*cmd)->append = 0;
	(*cmd)->redirect = 0;
	(*cmd)->infile_first = 0;
	(*cmd)->outfile_first = 0;
}