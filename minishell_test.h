/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_test.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:56:16 by shimi-be          #+#    #+#             */
/*   Updated: 2025/04/29 17:56:53 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef MINISHELL_TEST_H
#  define MINISHELL_TEST_H

#  include <stdio.h>
#  include <stdlib.h>
#  include <unistd.h>

typedef struct s_shell
{
	char			*type;
	char			*word;
	struct s_shell	*next;
}					t_shell;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

char				**ft_split(char const *s, char c);

# endif
