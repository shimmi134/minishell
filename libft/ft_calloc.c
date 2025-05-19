/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 18:12:32 by shimi-be          #+#    #+#             */
/*   Updated: 2024/09/27 13:23:26 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total_size;
	size_t	i;
	char	*memory;

	total_size = nmemb * size;
	if (total_size > 2147483647)
		return (NULL);
	memory = (char *)malloc(total_size);
	if (!memory)
		return (NULL);
	i = 0;
	while (total_size > i)
	{
		memory[i] = 0;
		i++;
	}
	return ((void *)memory);
}
