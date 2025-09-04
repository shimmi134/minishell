/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_struct_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:40:06 by shimi-be          #+#    #+#             */
/*   Updated: 2025/09/04 18:40:08 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	shift_left_and_free(char **arr)
{
	int	i;

	i = 0;
	if (!arr || !arr[0])
		return ;
	free(arr[0]);
	while (arr[i + 1])
	{
		arr[i] = arr[i + 1];
		i++;
	}
	arr[i] = NULL;
}
