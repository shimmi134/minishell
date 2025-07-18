/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:56:30 by shimi-be          #+#    #+#             */
/*   Updated: 2025/07/18 14:21:00 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_export(t_shell *elem, t_env **env)
{
	char	*str;
	t_env	*nd;
	int		i;
	t_env	*node;
	char	**split;

	if (!(ft_lensplit(elem->command->args) > 1))
	{
		split = ft_split(elem->command->args[0], '=');
		node = create_node(elem->command->args[0]);
		if (split)
		{
			str = split[1];
			for (int i = 2; i < ft_lensplit(split); i++)
			{
				if (split[i])
					str = ft_strjoin(str, "=");
				str = ft_strjoin(str, split[i]); // leaks
			}
			if (split[0][(int)ft_strlen(split[0]) - 1] != '+')
			{
				nd = (*env);
				while (nd)
				{
					if (!ft_strncmp(split[0], nd->key, ft_strlen(split[0])))
					{
						nd->value = str;
						break ;
					}
					nd = nd->next;
				}
				if (!nd)
				{
					node->value = str;
					addlast(env, node);
				}
			}
			else
			{
				nd = (*env);
				split[0] = ft_strtrim(split[0], "+");
				while (nd != NULL)
				{
					if (!ft_strncmp(nd->key, split[0], ft_strlen(split[0])))
					{
						nd->value = ft_strjoin(nd->value, str);
						free(str);
						break ;
					}
					nd = nd->next;
				}
				if (nd == NULL)
				{
					node->key = split[0];
					node->value = str;
					addlast(env, node);
				}
			}
		}
		else
		{
			nd = sort_list(env);
			while (nd)
			{
				printf("declare -x %s", nd->key);
				if (nd->value)
					printf("=\"%s\"", nd->value);
				printf("\n");
				nd = nd->next;
			}
		}
	}
	else
	{
		i = 1;
		while (i < ft_lensplit(elem->command->args))
		{
			printf("export: '%s': not a valid identifier\n",
				elem->command->args[i]);
			i++;
		}
		return (1);
	}
	return (0);
}

int	do_cd(t_shell *elem, t_env **env)
{
	char	*oldpwd;
	int		i;
	char	*str;
	t_env	*temp;
	int		d;

	if (elem->command->args[0] == NULL || !ft_strncmp("~\0",
			elem->command->args[0], 2))
	{
		temp = *env;
		if (!ft_strncmp("~\0", elem->command->args[0], 2))
			free(elem->command->args[0]);
		while (temp && ft_strncmp(temp->key, "HOME", 4) != 0)
			temp = temp->next;
		if (temp == NULL)
			return (printf("cd: HOME not set\n"), 1);
		else
			elem->command->args[0] = ft_strdup(temp->value);
	}
	oldpwd = getcwd(NULL, 0);
	i = chdir(elem->command->args[0]);
	if (i != -1)
	{
		str = getcwd(NULL, 0);
		temp = *env;
		while (temp)
		{
			if (!ft_strncmp(temp->key, "PWD", 3))
			{
				free(temp->value);
				temp->value = str;
			}
			else if (!ft_strncmp(temp->key, "OLDPWD", ft_strlen(temp->key)))
			{
				free(temp->value);
				temp->value = oldpwd;
			}
			temp = temp->next;
		}
	}
	else if (!ft_strncmp("-\0", elem->command->args[0], 2))
	{
		free(oldpwd);
		str = getcwd(NULL, 0);
		temp = *env;
		while (temp && ft_strncmp(temp->key, "OLDPWD",
				ft_strlen(temp->key)) != 0)
			temp = temp->next;
		oldpwd = temp->value;
		i = chdir(oldpwd);
		if (i != -1)
		{
			temp = *env;
			while (temp)
			{
				if (!ft_strncmp(temp->key, "PWD", 3))
				{
					free(temp->value);
					temp->value = oldpwd;
				}
				else if (!ft_strncmp(temp->key, "OLDPWD", ft_strlen(temp->key)))
				{
					temp->value = str;
				}
				temp = temp->next;
			}
		}
	}
	if (i == -1)
	{
		printf("cd: %s: %s\n", strerror(errno), elem->command->args[0]);
		return (1);
	}
	return (0);
}

int *find_newline(t_shell *elem)
{
	int *arr;
	int i;
	int newline;

	i = 0;
	arr = malloc(sizeof(int)*2);
	newline = 1;
	while (i < count_len(elem->command->args)
		&& !ft_strncmp(elem->command->args[i], "-n", 2)
		&& ft_strlen(elem->command->args[i]) == ft_strspn(elem->command->args[i],
			"-n"))
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
	int *arr;

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
		return (-1);
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
