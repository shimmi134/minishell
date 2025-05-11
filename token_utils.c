/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:06:45 by joshapir          #+#    #+#             */
/*   Updated: 2025/05/11 03:06:46 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int has_token(char *str)
{
	int i;

	i = 0;

	while (str[i])
	{
		if (is_token(str[i]))
			return(1);
		i++;
	}
	return(0);
}
