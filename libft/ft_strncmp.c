/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:10:31 by shimi-be          #+#    #+#             */
/*   Updated: 2024/09/27 17:02:42 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (n > i && (s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}
/*
int	main(void)
{
	char *s1 = "AAAAAA";
 	char *s2 = "";
 	int i1 = ((strncmp(s1, s2, 6) > 0) ? 1 : 
	((strncmp(s1, s2, 6) < 0) ? -1 : 0));
 	int i2 = ((ft_strncmp(s1, s2, 6) > 0) ? 1 : 
	((ft_strncmp(s1, s2, 6) < 0) ? -1 : 0));

	printf("i1: %i, i2: %i\n", i1, i2);
 	if (i1 == i2)
 		printf("yay\n");
 	printf("nay\n");
}*/
