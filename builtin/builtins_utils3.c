/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:01:45 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/26 18:55:59 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cd_home(t_shell *elem, t_env **env)
{
	t_env	*temp;

	temp = in_env("HOME", env);
	if (temp == NULL)
		return (printf("cd: HOME not set\n"), 1);
	else
		change_path(temp, elem);
	return (0);
}

void	change_env(t_env **env, char *oldpwd, char *str)
{
	t_env	*temp;

	temp = in_env("PWD", env);
	if (temp != NULL)
	{
		free(temp->value);
		temp->value = oldpwd;
	}
	temp = in_env("OLDPWD", env);
	if (temp != NULL)
	{
		temp->value = str;
	}
}

int	cd_back(t_env **env, char *oldpwd)
{
	char	*str;
	t_env	*temp;
	int		i;

	str = oldpwd;
	temp = in_env("OLDPWD", env);
	if (!temp)
		return (printf("OLDPWD not set.\n"), 1);
	oldpwd = temp->value;
	i = chdir(oldpwd);
	if (i != -1)
	{
		change_env(env, oldpwd, str);
		return (0);
	}
	else
		return (printf("OLDPWD not set correctly.\n"), 1);
}

int	cd_correct(t_env **env, char *oldpwd)
{
	char	*str;
	t_env	*temp;

	str = getcwd(NULL, 0);
	temp = in_env("PWD", env);
	if (temp != NULL)
	{
		free(temp->value);
		temp->value = str;
	}
	else
		free(str);
	temp = in_env("OLDPWD", env);
	if (temp != NULL)
	{
		free(temp->value);
		temp->value = oldpwd;
	}
	else
		free(oldpwd);
	return (0);
}

int	do_cd(t_shell *elem, t_env **env)
{
	char	*oldpwd;
	int		i;

	if (elem->command->args[0] != NULL && elem->command->args[1] != NULL)
		return (printf("cd: too many arguments.\n"), 1);
	if (elem->command->args[0] == NULL || !ft_strncmp("~\0",
			elem->command->args[0], 1))
	{
		i = cd_home(elem, env);
		if (i == 1)
			return (1);
	}
	oldpwd = getcwd(NULL, 0);
	i = chdir(elem->command->args[0]);
	if (i != -1)
		i = cd_correct(env, oldpwd);
	else if (!ft_strncmp("-\0", elem->command->args[0], 2))
		i = cd_back(env, oldpwd);
	if (i == -1)
		return (printf("cd: %s: %s\n", strerror(errno), elem->command->args[0]),
			1);
	if ((*env)->pwd_copy)
		free((*env)->pwd_copy);
	(*env)->pwd_copy = ft_strdup(in_env("PWD", env)->value);
	return (i);
}
