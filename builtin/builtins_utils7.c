/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils7.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 17:22:09 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/30 17:22:11 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	copy_pwd_env_p(t_env **env, t_env *node, int flag)
{
	if (node->pwd_copy)
		free(node->pwd_copy);
	node->pwd_copy = ft_strdup(in_env("PWD", env)->value);
	if (node->hidden == 2)
		node->hidden = 0;
	if (ft_strcmp("OLDPWD", node->key) == 0 && flag)
	{
		if (node->value)
			free(node->value);
		node->value = delete_last(node->pwd_copy);
	}
}

int	copy_pwd_env_np(t_env *node)
{
	char	*str;
	char	*str2;

	str = node->pwd_copy;
	node->pwd_copy = ft_strjoin(node->pwd_copy, "/..");
	if (ft_strcmp("PWD", node->key) == 0)
	{
		str2 = node->value;
		node->value = ft_strdup(node->pwd_copy);
		free(str2);
	}
	free(str);
	return (1);
}

void	pwd_copy_for_env(t_env **env)
{
	t_env	*node;
	int		flag;

	node = *env;
	flag = 0;
	while (node)
	{
		if (in_env("PWD", env)->value != NULL)
			copy_pwd_env_p(env, node, flag);
		else
			flag = copy_pwd_env_np(node);
		node = node->next;
	}
}

char	*ft_strrchr(const char *s, int c)
{
	const char	*chr;

	chr = NULL;
	while (*s)
	{
		if ((unsigned char)*s == (unsigned char)c)
			chr = s;
		s++;
	}
	if ((unsigned char)c == '\0')
		return ((char *)s);
	return ((char *)chr);
}

char	*delete_last(char *str)
{
	char	*str2;
	int		len;
	int		i;

	str2 = ft_strrchr(str, '/');
	len = str2 - str;
	str2 = malloc(len + 1);
	if (!str2)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str2[i] = str[i];
		i++;
	}
	str2[i] = '\0';
	return (str2);
}
