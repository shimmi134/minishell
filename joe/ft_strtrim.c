/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 22:25:30 by joshapir          #+#    #+#             */
/*   Updated: 2025/06/30 17:39:43 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_set(char const s1, char const *set)
{
	int	j;

	j = 0;
	while (s1 && set[j])
	{
		if (s1 == set[j])
			return (1);
		j++;
	}
	return (0);
}

char	*ft_trim_all(int start, int end)
{
	char	*arr;

	arr = "0";
	if (start == end + 1)
	{
		arr = (char *)malloc(1);
		arr[0] = '\0';
		return (arr);
	}
	return (arr);
}

char	*ft_trim(char *arr, int start, int end, char const *s1)
{
	int	i;

	i = 0;
	while (start <= end)
		arr[i++] = s1[start++];
	arr[i] = '\0';
	return (arr);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*arr;
	int		i;
	int		start;
	int		end;

	start = 0;
	end = ft_strlen(s1) - 1;
	i = 0;
	if (!s1 || !set)
		return (NULL);
	while (s1[start] && find_set(s1[start], set))
		start++;
	if (start == end + 1)
		return (ft_trim_all(start, end));
	while (end >= 0 && find_set(s1[end], set))
		end--;
	if (end < start - 2)
		return (NULL);
	arr = (char *)malloc(end - start + 2);
	if (!arr)
		return (NULL);
	arr = ft_trim(arr, start, end, s1);
	return (ft_trim(arr, start, end, s1));
}

/*int main (void)
{
//  char i[] = "abc";
//  char s1[] = "abceeehello worldabc";
  char *temp;

  temp = ft_strtrim("   xxx   xxx", " x");
  printf("%s", temp);
  free (temp);

  return (0);	
}*/
