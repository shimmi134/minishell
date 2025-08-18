/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 14:56:49 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/18 14:58:46 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	perr_exit(int errnum, char *cmd)
{
	perror(cmd);
	exit(errnum);
}

t_env	*duplicate_env(t_env *env)
{
	t_env	*dup;
	t_env	*head;
	t_env	*last;

	head = NULL;
	dup = NULL;
	last = NULL;
	while (env != NULL)
	{
		dup = malloc(sizeof(t_env));
		if (!dup)
			return (NULL);
		dup->key = ft_strdup(env->key);
		dup->value = ft_strdup(env->value);
		dup->next = NULL;
		env = env->next;
		if (head == NULL)
			head = dup;
		else
			last->next = dup;
		last = dup;
	}
	return (head);
}

int	swap(t_env *temp)
{
	char	*key;
	char	*value;

	if (ft_strncmp(temp->key, temp->next->key, ft_strlen(temp->key)) > 0)
	{
		key = temp->key;
		value = temp->value;
		temp->key = temp->next->key;
		temp->value = temp->next->value;
		temp->next->key = key;
		temp->next->value = value;
		return (1);
	}
	return (0);
}

void	sort_list(t_env *env)
{
	t_env	*temp;
	t_env	*head;
	int		swapped;

	if (env == NULL)
		return ;
	head = duplicate_env(env);
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		temp = head;
		while (temp && temp->next)
		{
			swapped = swap(temp);
			temp = temp->next;
		}
	}
	print_env(head);
	free_env_list_tmp(head);
}

void	print_env(t_env *head)
{
	while (head != NULL)
	{
		printf("declare -x %s", head->key);
		if (head->value)
			printf("=\"%s\"", head->value);
		printf("\n");
		head = head->next;
	}
}
