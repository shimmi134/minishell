/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 21:28:54 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/21 14:52:28 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_while(char *arr, char const *s1, char const *s2)
{
	int	first;
	int	total;
	int	i;

	i = 0;
	first = ft_strlen(s1);
	total = ft_strlen(s1) + ft_strlen(s2);
	while (i < first)
	{
		arr[i] = s1[i];
		i++;
	}
	while (first < total)
	{
		arr[first++] = *s2;
		s2++;
	}
	arr[first] = '\0';
	return (arr);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		total;
	int		i;
	char	*arr;

	if (!s2)
		return (NULL);
	total = ft_strlen(s1) + ft_strlen(s2);
	i = 0;
	arr = malloc(total + 1);
	if (!arr)
		return (NULL);
	return (ft_while(&arr[i], s1, s2));
}
