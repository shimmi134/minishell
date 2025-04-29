/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:09:48 by shimi-be          #+#    #+#             */
/*   Updated: 2024/09/22 16:41:16 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	i;
	char	*dst;
	char	*sc;

	if (!dest && !src)
		return (NULL);
	dst = (char *)dest;
	sc = (char *)src;
	if (dst > sc && dest < src + n)
	{
		i = n;
		while (i > 0)
		{
			dst[i - 1] = sc[i - 1];
			i--;
		}
	}
	else
	{
		i = 0;
		while (n > i++)
			dst[i - 1] = sc[i - 1];
	}
	return (dest);
}
