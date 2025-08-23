/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils11.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 21:47:17 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/23 21:48:04 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*correct_shlvl(char *split)
{
	if (ft_abs(ft_atoi(split)) > 1000)
	{
		free(split);
		return (ft_strdup("0"));
	}
	return (free(split), ft_itoa(ft_atoi(split) + 1));
}

int	ft_abs(int num)
{
	if (num >= 0)
		return (num);
	else
		return (-num);
}
