/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils6.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:40:58 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/23 16:48:49 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	valiaa(char *str)
{
	int	i;
	int	sep;

	i = 0;
	sep = 0;
	while (i < (int)ft_strlen(str))
	{
		if (str[i] == 'n')
			sep = 1;
		else if (sep && str[i] == '-')
			return (0);
		else if (!sep && str[i] == 'n')
			return (0);
		else if (i != 0 && str[i] == '-' && str[i - 1] == '-')
			return (0);
		i++;
	}
	return (1);
}

int	len_spn(char *str)
{
	return ((int)ft_strlen(str) == ft_strspn(str, "-n"));
}
