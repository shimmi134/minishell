/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:56:30 by shimi-be          #+#    #+#             */
/*   Updated: 2025/07/10 19:04:12 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int do_export(t_shell *elem, t_env** env)
{
	char	*str;
	t_env	*nd;
	int		i;
	t_env	*node;
	char 	**split;

	if (!(ft_lensplit(elem->command->args) > 1))
	{
		split = ft_split(elem->command->args[0], '=');
		node = create_node(elem->command->args[0]);
		if (split)
		{
			str = split[1];
			for (int i = 2;  i < ft_lensplit(split); i++)
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
	else {
		i = 1;
		while (i < ft_lensplit(elem->command->args))
		{
			printf("export: '%s': not a valid identifier\n", elem->command->args[i]);
			i++;
		}
		return 1;
	}
	return 0;
}
