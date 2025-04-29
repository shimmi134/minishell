/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:09:42 by shimi-be          #+#    #+#             */
/*   Updated: 2024/09/27 17:25:58 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t		i;
	char		*dst;
	const char	*sc;	

	if (!dest && !src)
		return (NULL);
	i = 0;
	dst = (char *)dest;
	sc = (const char *)src;
	while (n > i)
	{
		dst[i] = sc[i];
		i++;
	}
	return (dest);
}
