/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:10:03 by shimi-be          #+#    #+#             */
/*   Updated: 2024/09/27 13:21:53 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	len_dst;
	size_t	len_src;

	i = 0;
	len_dst = ft_strlen(dst);
	len_src = ft_strlen(src);
	if (size <= len_dst)
		return (size + len_src);
	while (src[i] && i < (size - len_dst - 1))
	{
		dst[len_dst + i] = src[i];
		i++;
	}
	dst[len_dst + i] = '\0';
	return (len_dst + len_src);
}
/*
#include <bsd/string.h>
	
int	main(int ac, char **av)
{
	char *helper;

	helper = ft_strdup(av[1]);	
	if (ac == 4)
	{
printf("Mine: %zu, %s\n", ft_strlcat(helper, av[2], 
ft_atoi(av[3])), helper);
printf("Original: %zu, %s\n", strlcat(av[1], av[2],
ft_atoi(av[3])), av[1]);
	}
	return 0;
}*/
