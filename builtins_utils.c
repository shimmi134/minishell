/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:56:30 by shimi-be          #+#    #+#             */
/*   Updated: 2025/07/19 16:51:42 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *fun(char *str, char c)
{
	int i = 0;
	while (str && str[i] != c)
	{
		i++;
	}
	char *h = malloc(i+1);
	h[i] = '\0';
	int j = 0;
	while (j < i)
	{
		h[j] = str[j];
		j++;
	}
	return h;
}

int correct_export(char *str)
{
	int	i;
	int len;

	i = 0;
	len = ft_strlen(str);
	while (i < len)
	{
		if (!ft_isalnum(str[i]) && str[i] != '=')
			return (-1);
		if (ft_isdigit(str[i]) && i == 0)
			return (-1);
		i++;
	}
	return (1);
}

int	in_env(char *str, t_env **env)
{
	t_env *nd;

	nd = *env;
	while (nd)
	{
		if (ft_strncmp(str, nd->key, ft_strlen(str)) == 0)
			return (1);
		nd = nd->next;
	}
	return (0);
}

int	do_export(t_shell *elem, t_env **env)
{
	char	*str;
	t_env	*nd;
	int		i;
	t_env	*node;
	char	*pos;
	int		rv;
	
	rv = 0;
	if ((ft_lensplit(elem->command->args) >= 1))
	{
		i = 0;
		while (i < ft_lensplit(elem->command->args))
		{
			str = ft_strchr(elem->command->args[i], '=');
			if (correct_export(elem->command->args[i]) == -1)
			{
				printf("export: '%s': not a valid identifier\n", elem->command->args[i]);
				rv = 1;
			}
			else if (str != NULL && *(str-1) != '+' && correct_export(elem->command->args[i]))
			{
				nd = *env;
				pos = fun(elem->command->args[i], '=');
				while (nd)
				{
					if (ft_strncmp(pos, nd->key, ft_strlen(nd->key)) == 0)
					{
						free(nd->value);
						nd->value = ft_strdup((str+1));
						break;
					}
					nd = nd->next;
				}
				free(pos);
				if (!nd)
				{
					node = malloc(sizeof(t_env));
					if (!node)
						return (-1);
					node->key = fun(elem->command->args[i], '=');
					node->value = ft_strdup((str+1));
					node->next = NULL;
					addlast(env,node);
				}
			}
			else if (str == NULL && !in_env(elem->command->args[i], env) && correct_export(elem->command->args[i]))
			{
				node = malloc(sizeof(t_env));
				node->key = ft_strdup(elem->command->args[i]);
				node->value = NULL;
				node->next = NULL;
				addlast(env, node);
			}
			i++;
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
	return (rv);
}

int	do_cd(t_shell *elem, t_env **env)
{
	char	*oldpwd;
	int		i;
	char	*str;
	t_env	*temp;
	int		d;

	if (elem->command->args[0] == NULL || !ft_strncmp("~\0",
			elem->command->args[0], 1))
	{
		temp = *env;
		while (temp && ft_strncmp(temp->key, "HOME", 4) != 0)
			temp = temp->next;
		if (temp == NULL)
			return (printf("cd: HOME not set\n"), 1);
		else
		{
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
					elem->command->args[0] = ft_strdup(temp->value);
			}
			else 
				elem->command->args[0] = ft_strdup(temp->value);
			
		}
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
