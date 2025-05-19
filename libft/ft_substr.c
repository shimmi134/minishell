/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 12:34:06 by shimi-be          #+#    #+#             */
/*   Updated: 2024/09/27 16:32:40 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	j;
	char	*arr;

	if (s == NULL)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	j = 0;
	if (start + len > ft_strlen(s))
		len = ft_strlen(s) - start;
	arr = (char *)malloc(len * sizeof(char) + 1);
	if (arr == NULL)
		return (NULL);
	while (len > j)
	{
		arr[j] = s[j + start];
		j++;
	}
	arr[j] = '\0';
	return (arr);
}
