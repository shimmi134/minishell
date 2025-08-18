/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils6.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:21:40 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/18 15:26:46 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*copy_env(char *envp[])
{
	t_env	*head;
	t_env	*tail;
	t_env	*new_node;
	int		i;
	int		shlvl;

	init_env_vals(&head, &i, &shlvl);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
			shlvl = 1;
		new_node = create_node(envp[i]);
		if (!new_node)
			return (NULL);
		if (!head)
			head = new_node;
		else
			tail->next = new_node;
		tail = new_node;
		i++;
	}
	if (!shlvl)
		create_shlvl(tail);
	return (head);
}

void	free_split(char **sp)
{
	int	i;

	i = 0;
	while (sp && sp[i])
	{
		free(sp[i]);
		i++;
	}
	if (sp)
		free(sp);
}

char	*get_element(char *line)
{
	char	**split;

	split = ft_split(line, ' ');
	if (split && (!ft_strncmp(split[0], "pwd", 4) || !ft_strncmp(split[0],
				"env", 4) || !ft_strncmp(split[0], "unset", 6)
			|| !ft_strncmp(split[0], "echo", 5) || !ft_strncmp(split[0],
				"export", 6) || !ft_strncmp(split[0], "cd", 2)))
	{
		return (free_split(split), "built-in");
	}
	else
	{
		return (free_split(split), "command");
	}
}

void	do_struct(t_shell **element, t_cmd *command, int *exit_status)
{
	t_shell	*new_node;
	t_shell	*last;

	*element = NULL;
	last = NULL;
	while (command)
	{
		new_node = malloc(sizeof(t_shell));
		if (!new_node)
			exit(1);
		new_node->command = command;
		new_node->type = get_element(command->cmd);
		new_node->exit_status_code = exit_status;
		new_node->next = NULL;
		if (!*element)
			*element = new_node;
		if (last)
			last->next = new_node;
		last = new_node;
		command = command->next;
	}
}

void	free_shell(t_shell *element)
{
	t_shell	*temp;

	while (element)
	{
		temp = element;
		element = element->next;
		free(temp);
	}
}
