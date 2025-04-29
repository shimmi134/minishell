/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:10:28 by shimi-be          #+#    #+#             */
/*   Updated: 2024/09/28 14:31:18 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <string.h>

char	*ft_strrchr(const char *s, int c)
{
	const char	*chr;

	chr = NULL;
	while (*s)
	{
		if ((unsigned char)*s == (unsigned char)c)
			chr = s;
		s++;
	}
	if ((unsigned char)c == '\0')
		return ((char *)s);
	return ((char *)chr);
}
/*	
int main()
{
	const char *s = "teste";
	int c = 1024;

	printf("tu funcion :::: %p\n", ft_strrchr(s, c));
	printf("la funcion :::: %p\n", strrchr(s, c));
}*/
