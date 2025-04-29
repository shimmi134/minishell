/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 14:47:04 by shimi-be          #+#    #+#             */
/*   Updated: 2024/09/27 16:40:45 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*domal(int len, int n)
{
	unsigned int		nb;
	char				*num;

	num = (char *) malloc (len + 1);
	if (!num)
		return (NULL);
	num[len] = '\0';
	if (n < 0)
	{
		num[0] = '-';
		nb = -(unsigned int)n;
	}
	else if (n == 0)
	{
		num[0] = '0';
		return (num);
	}
	else
		nb = (unsigned int)n;
	while (nb > 0)
	{
		num[--len] = (nb % 10) + '0';
		nb /= 10;
	}
	return (num);
}

char	*ft_itoa(int n)
{
	int		len;

	len = get_len(n);
	return (domal(len, n));
}
