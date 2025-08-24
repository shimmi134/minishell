/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils11.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 21:47:17 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/24 14:08:38 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*correct_shlvl(char *split)
{
	char	*val;

	if (ft_abs(ft_atoi(split)) > 1000)
	{
		free(split);
		return (ft_strdup("0"));
	}
	val = ft_itoa(ft_atoi(split)+1);
	return (free(split), val);
}

int	ft_abs(int num)
{
	if (num >= 0)
		return (num);
	else
		return (-num);
}

int	custom_exit(int code, int *exit_status, t_env *env, t_cmd *t_head)
{
	free_cmds(t_head);
	clear_history();
	free_combined(exit_status, env);
	exit(code);
}
