/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 17:47:58 by shimi-be          #+#    #+#             */
/*   Updated: 2025/05/19 16:00:13 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	len(char const *s, char c, int counter)
{
	int	length;

	length = 0;
	while (s[counter] != c && s[counter])
	{
		counter++;
		length++;
	}
	return (length);
}

static int	skipchar(char const *s, char c, int i)
{
	while (s[i] == c && s[i])
		i++;
	return (i);
}

static int	cwords(char const *s, char c)
{
	int	i;
	int	count;
	int	in_word;

	count = 0;
	i = 0;
	in_word = 0;
	while (s[i])
	{
		if (s[i] == c)
		{
			if (in_word > 0)
			{
				count++;
				in_word = 0;
			}
		}
		else
			in_word = 1;
		i++;
	}
	if (in_word)
		count++;
	return (count);
}

static char	**domalloc(int *w, char **arr, char const *s, char c)
{
	int	size;
	int	i;
	int	j;

	i = 0;
	while (i < w[0])
	{
		j = 0;
		size = len(s, c, w[1]);
		arr[i] = (char *) malloc (size + 1);
		if (!arr[i])
		{
			while (i > 0)
				free(arr[i--]);
			free(arr[i]);
			free(arr);
			return (NULL);
		}
		arr[i][size] = '\0';
		while (j < size)
			arr[i][j++] = s[w[1]++];
		i++;
		w[1] = skipchar(s, c, w[1]);
	}
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	int		w[2];

	if (!s)
		return (NULL);
	w[0] = cwords(s, c);
	arr = (char **) malloc ((w[0] + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	arr[w[0]] = NULL;
	w[1] = skipchar(s, c, 0);
	return (domalloc(w, arr, s, c));
}
