/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 12:40:19 by shimi-be          #+#    #+#             */
/*   Updated: 2024/09/28 14:31:02 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"

int	str_end(char *str, char const *set)
{
	int	len;
	int	i;
	int	j;
	int	flag;

	len = ft_strlen(str);
	i = 0;
	while (len > i)
	{
		flag = 0;
		j = 0;
		while (set[j])
		{
			if (set[j] != str[len - i - 1])
				flag++;
			j++;
		}
		if (flag == (int)ft_strlen(set))
			return (len - i);
		i++;
	}
	return (0);
}

int	str_start(char *str, char const *set)
{
	int	i;
	int	j;
	int	flag;

	i = 0;
	while (str[i])
	{
		flag = 0;
		j = 0;
		while (set[j])
		{
			if (set[j] != str[i])
				flag++;
			j++;
		}
		if (flag == (int)ft_strlen(set))
			return (i);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*arr;
	char	*str;
	int		start;
	int		end;
	int		i;

	if (s1 == NULL || set == NULL)
		return (NULL);
	str = (char *)s1;
	start = str_start(str, set);
	end = str_end(str, set);
	arr = (char *)malloc (end - start + 1);
	if (!arr)
		return (NULL);
	i = 0;
	while (end > start)
	{
		arr[i] = s1[start];
		i++;
		start++;
	}
	arr[i] = '\0';
	return (arr);
}

/*
int main (int ac, char *av[])
{
	(void) ac;
	char const *set = " ";
	char const *s1 = av[1];
	char *s2 = ft_strtrim(s1, set);
	printf("la longitud de s2 es %zu\n", ft_strlen(s2));

	printf("%s\n", s2);
}*/
