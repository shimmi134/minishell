/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:07:51 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/19 18:21:27 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_path(t_env *temp, t_shell *elem)
{
	char	*str;
	char	*oldpwd;

	if (!ft_strncmp("~\0", elem->command->args[0], 1))
	{
		str = elem->command->args[0];
		if (ft_strchr(elem->command->args[0], '/') != NULL)
		{
			oldpwd = ft_strchr(elem->command->args[0], '/');
			elem->command->args[0] = ft_strjoin(temp->value, oldpwd);
			free(str);
		}
		else
		{
			free(elem->command->args[0]);
			elem->command->args[0] = ft_strdup(temp->value);
		}
	}
	else
		elem->command->args[0] = ft_strdup(temp->value);
}

int valiaa(char *str)
{
	int	i;
	int	sep;

	i = 0;
	sep = 0;
	while (i < ft_strlen(str))
	{
		if (str[i] == 'n')
			sep = 1;
		else if (sep && str[i] == '-')
			return (0);
		else if (!sep && str[i] == 'n')
			return (0);
		else if (i != 0 && str[i] == '-' && str[i-1] == '-')
			return (0);
		i++;
	}
	return (1);
}

int	*find_newline(t_shell *elem)
{
	int	*arr;
	int	i;
	int	newline;

	i = 0;
	arr = malloc(sizeof(int) * 2);
	newline = 1;
	while (i < count_len(elem->command->args)
		&& !ft_strncmp(elem->command->args[i], "-n", 2)
		&&ft_strlen(elem->command->args[i]) == ft_strspn(elem->command->args[i],
			"-n") && valiaa(elem->command->args[i]))
	{
		newline = 0;
		i++;
	}
	arr[0] = newline;
	arr[1] = i;
	return (arr);
}

int	do_echo(t_shell *elem, t_env **env)
{
	int	i;
	int	newline;
	int	*arr;

	arr = find_newline(elem);
	i = arr[1];
	newline = arr[0];
	if (elem->command->exit_status == 0)
	{
		while (i < count_len(elem->command->args))
		{
			printf("%s", elem->command->args[i]);
			if (i < count_len(elem->command->args) - 1)
				printf(" ");
			i++;
		}
	}
	else
		printf("%i", *(elem->exit_status_code));
	if (newline)
		printf("\n");
	free(arr);
	return (0);
}

int	do_pwd(void)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	if (!buf)
		return (perror("getcwd"), 1);
	else if (printf("%s\n", buf) == -1)
		return (1);
	free(buf);
	return (0);
}

int	do_env(t_env **env)
{
	t_env	*nd;

	nd = (*env);
	while (nd)
	{
		if (nd->value != NULL)
			printf("%s=%s\n", nd->key, nd->value);
		nd = nd->next;
	}
	return (0);
}
