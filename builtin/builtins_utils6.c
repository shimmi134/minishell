/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils6.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:40:58 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/23 16:48:49 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	valiaa(char *str)
{
	int	i;
	int	sep;

	i = 0;
	sep = 0;
	while (i < (int)ft_strlen(str))
	{
		if (str[i] == 'n')
			sep = 1;
		else if (sep && str[i] == '-')
			return (0);
		else if (!sep && str[i] == 'n')
			return (0);
		else if (i != 0 && str[i] == '-' && str[i - 1] == '-')
			return (0);
		i++;
	}
	return (1);
}

int	len_spn(char *str)
{
	return ((int)ft_strlen(str) == ft_strspn(str, "-n"));
}

int	export_loop(t_shell *elem, t_env **env)
{
	int		i;
	char	*str;
	int		ret_val;

	i = 0;
	ret_val = 0;
	while (i < ft_lensplit(elem->command->args))
	{
		str = ft_strchr(elem->command->args[i], '=');
		if (correct_export(elem->command->args[i]) == -1)
			ret_val = export_error(elem->command->args[i]);
		else if (str != NULL && *(str - 1) != '+')
			good_export(elem->command->args[i], env, str);
		else if (str == NULL && !in_env(elem->command->args[i], env)
			&& correct_export(elem->command->args[i]))
			create_and_add(env, elem->command->args[i]);
		else if (str == NULL && in_env(elem->command->args[i], env)
			&& correct_export(elem->command->args[i]))
			change_flag(elem->command->args[i], env);
		i++;
	}
	return (ret_val);
}

int	was_hidden(t_env *node)
{
	if (node->hidden)
	{
		node->hidden = 0;
		return (1);
	}
	return (0);
}

void	change_flag(char	*str, t_env **env)
{
	t_env	*node;

	node = in_env(str, env);
	node->hidden = 2;
}
