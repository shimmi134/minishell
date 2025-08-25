/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:07:51 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/25 22:21:13 by shimi-be         ###   ########.fr       */
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
		&& len_spn(elem->command->args[i]) && valiaa(elem->command->args[i]))
	{
		newline = 0;
		i++;
	}
	arr[0] = newline;
	arr[1] = i;
	return (arr);
}

int	do_echo(t_shell *elem)
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

int	do_pwd(t_env **env)
{
	char	*buf;
	t_env	*temp;

	buf = getcwd(NULL, 0);
	temp = in_env("PWD", env); 
	if (!buf)
		buf = ft_strdup(temp->value);
	if (printf("%s\n", buf) == -1)
		return (1);
	if (buf)
		free(buf);
	return (0);
}

int	do_env(t_env **env, t_shell *elem)
{
	t_env	*nd;
	int		v;

	nd = (*env);
	v = incorr_env(elem);
	if (v != 0)
	{
		if (v == 1)
			return (printf("Error copying the env.\n"), 1);
		else if (v == 2)
			return (127);
		else
			return (0);
	}
	while (nd)
	{
		if (nd->value != NULL && !nd->hidden)
			printf("%s=%s\n", nd->key, nd->value);
		nd = nd->next;
	}
	return (0);
}
