/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:09:33 by shimi-be          #+#    #+#             */
/*   Updated: 2024/09/27 13:25:48 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	result = 0;
	sign = 1;
	while ((nptr[i] >= '\t' && nptr[i] <= '\r') || nptr[i] == 32)
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -sign;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	return (result * sign);
}
/*
#include <stdlib.h>
#include <stdio.h>

int	main(void)
{
	printf("ft_atoi: %i\n",ft_atoi("123456"));
	printf("atoi: %i\n", atoi("123456"));
	printf("ft_atoi: %i\n",ft_atoi("    -123456"));
	printf("atoi: %i\n", atoi("    -123456"));
	printf("3 ft_atoi: %i\n", ft_atoi("-a123456"));
	printf("3 atoi: %i\n", atoi("-a123456"));
	printf("ft_atoi: %i\n", ft_atoi("   -123aa456"));
	printf("atoi: %i\n", atoi("   -123aa456"));
	printf("ft_atoi: %i\n", ft_atoi("-2147483649"));
	printf("atoi: %i\n", atoi("-2147483649"));
}*/
