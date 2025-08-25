/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 18:53:16 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/25 22:10:28 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_var(char *str, t_cmd *cmd, t_env *env)
{
	char	*val;
	int		i;

	i = 0;
	while (env)
	{
		if (!ft_strcmp(str, env->key))
		{
			if (env->hidden)
				return(NULL);
			val = ft_strdup(env->value);
			return (val);
		}
		else if (cmd && str[0] == '?' && !str[1])
		{
			cmd->exit_status2 = 1;
			return (ft_itoa(cmd->exit_code));
		}
		env = env->next;
		i++;
	}
	return (NULL);
}
