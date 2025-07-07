/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 12:30:28 by shimi-be          #+#    #+#             */
/*   Updated: 2024/09/19 17:11:29 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined;
	int		size_s;
	int		i;
	int		j;

	size_s = ft_strlen(s1) + ft_strlen(s2);
	joined = (char *) malloc (size_s * sizeof(char) + 1);
	if (!joined)
		return (NULL);
	i = 0;
	j = 0;
	while (s1 && s1[i])
	{
		joined[i] = s1[i];
		i++;
	}
	while (s2 && s2[j])
	{
		joined[i] = s2[j];
		i++;
		j++;
	}
	joined[i] = '\0';
	return (joined);
}
/*
int main(void){
	char *s1 = "hello";
	char *s2 = NULL;

	char *s3 = ft_strjoin(s1,s2);
	printf("%s\n", s3);
}
*/
