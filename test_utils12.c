/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils12.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 16:55:28 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/25 15:58:22 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_sign(int c)
{
	return (c == '+' || c == '-');
}

int	is_sal(char *str)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (str[i])
	{
		if (i == 1)
			flag = 1;
		if (is_sign(str[i]) && flag)
			return (0);
		if (!ft_isdigit(str[i]) && !is_sign(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	exit_message(t_shell *elem)
{
	if (ft_strchr(elem->command->cmd, '/') != NULL && ft_strncmp("exit", elem->command->cmd, 4) != 0)
		perror(elem->command->cmd);
	else if (ft_strncmp("exit", elem->command->cmd, 4) != 0)
		printf("%s: command not found\n", elem->command->cmd);
	exit(127);
}
