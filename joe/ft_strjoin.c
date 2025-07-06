/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 21:28:54 by joshapir          #+#    #+#             */
/*   Updated: 2025/07/04 13:32:23 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
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
/*int main()
  {
// Test cases
char	*str1 = "Hello, ";
char	*str2 = "World!";
char	*result;

// Test 1: Normal concatenation
result = ft_strjoin(str1, str2);
if (result)
{
printf("Test 1: %s + %s = %s\n", str1, str2, result);
free(result);  // Free the dynamically allocated memory
}

// Test 2: Empty string concatenation
str1 = "";
str2 = "World!";
result = ft_strjoin(str1, str2);
if (result)
{
printf("Test 2: %s + %s = %s\n", str1, str2, result);
free(result);
}

// Test 3: Both strings are empty
str1 = "";
str2 = "";
result = ft_strjoin(str1, str2);
if (result)
{
printf("Test 3: %s + %s = %s\n", str1, str2, result);
free(result);
}

// Test 4: One NULL input (should handle gracefully)
str1 = NULL;
str2 = "World!";
result = ft_strjoin(str1, str2);
if (result)
{
printf("Test 4: NULL + %s = %s\n", str2, result);
free(result);
}
else
{
printf("Test 4: NULL input handled properly.\n");
}

// Test 5: Both NULL inputs (should return NULL)
str1 = NULL;
str2 = NULL;
result = ft_strjoin(str1, str2);
if (!result)
{
printf("Test 5: NULL + NULL = NULL (handled correctly)\n");
}

return (0);
}*/
