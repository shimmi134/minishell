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

char *ft_dup_upto(char *str, char c)
{
	int i;
	int j;
	char *helper;

	i = 0;
	j = 0;
	while (str && str[i] != c)
		i++;
	helper = malloc(i+1);
	helper[i] = '\0';
	while (j < i)
	{
		helper[j] = str[j];
		j++;
	}
	return helper;
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

t_env*	in_env(char *str, t_env **env)
{
	t_env *nd;

	nd = *env;
	while (nd)
	{
		if (ft_strncmp(str, nd->key, ft_strlen(str)) == 0)
			return (nd);
		nd = nd->next;
	}
	return (NULL);
}

void good_export(char *arg, t_env **env, char* str)
{
	t_env	*nd;
	char	*pos;

	nd = *env;
	pos = ft_dup_upto(arg, '=');
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
		nd = create_env_node(arg,env, 1, str);
		addlast(env,nd);
	}
}

int export_error(char *arg)
{
	printf("export: '%s': not a valid identifier\n", arg);
	return 1;
}


int	do_export(t_shell *elem, t_env **env)
{
	char	*str;
	int		i;
	t_env	*node;
	int		ret_val;
	
	ret_val = 0;
	if ((ft_lensplit(elem->command->args) >= 1))
	{
		i = 0;
		while (i < ft_lensplit(elem->command->args))
		{
			str = ft_strchr(elem->command->args[i], '=');
			if (correct_export(elem->command->args[i]) == -1)
				ret_val = export_error(elem->command->args[i]);	
			else if (str != NULL && *(str-1) != '+' && correct_export(elem->command->args[i]))
				good_export(elem->command->args[i], env, str);
			else if (str == NULL && !in_env(elem->command->args[i], env) && correct_export(elem->command->args[i]))
				create_and_add(env, elem->command->args[i]);
			i++;
		}
	}
	else
		sort_list(*env);
	return (ret_val);
}

void create_and_add(t_env **env, char *str)
{
	t_env *node;
	
	node = create_env_node(str,env, 0, NULL);
	addlast(env, node);
}

t_env* create_env_node(char *arg, t_env **env, int flag, char *str)
{
	t_env *node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	if (!flag)
	{
		node->key = ft_strdup(arg);
		node->value = NULL;
		node->next = NULL;
	}
	else
	{
		node->key = ft_dup_upto(arg, '=');
		node->value = ft_strdup((str+1));
		node->next = NULL;
	}
	return (node);
}

int cd_home(t_shell *elem, t_env **env)
{
	t_env *temp;
	char *str;
	char *oldpwd;

	temp = in_env("HOME", env);
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
	return (0);
}

void change_env(t_env **env, char *oldpwd, char *str)
{
	t_env *temp;

	temp = in_env("PWD", env);
	if (temp != NULL)
	{
		free(temp->value);
		temp->value = oldpwd;
	}
	temp = in_env("OLDPWD", env);
	if (temp != NULL)
	{
		//free(temp->value); Maybe, im not sure.
		temp->value = str;
	}
}
int cd_back(t_shell *elem, t_env **env, char *oldpwd)
{
	char *str;
	t_env *temp;
	int i;

	str = oldpwd;
	temp = in_env("OLDPWD", env);
	if (!temp)
		return (printf("OLDPWD not set.\n"), 1);
	oldpwd = temp->value;
	i = chdir(oldpwd);
	if (i != -1)
	{
		change_env(env, oldpwd, str);
		return 0;
	}
	else
		return (printf("OLDPWD not set correctly.\n"), 1);
}

int cd_correct(t_env **env, char *oldpwd)
{
	char *str;
	t_env *temp;

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
		i = cd_back(elem, env, oldpwd);
	if (i == -1)
		return (printf("cd: %s: %s\n", strerror(errno), elem->command->args[0]), 1);
	return (i);
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
