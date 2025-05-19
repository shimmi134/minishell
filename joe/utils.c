/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:07:16 by joshapir          #+#    #+#             */
/*   Updated: 2025/05/19 16:01:47 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *ft_strdup_char (char c)
{
    int i;
    char *dup;

    i = 0;
    dup = malloc(sizeof(char ) * + 1);
    i = 0;
    dup[0] = c;
    dup[1] = '\0';
    return(dup);
}
char *ft_strdup (char *str)
{
    int i;
    char *dup;

    i = 0;
    while(str[i])
        i++;
    dup = malloc(sizeof(char *) * (i + 1));
    i = 0;
    while(str[i])
    {
        dup[i] = str[i];
        i++;
    }
    dup[i] = '\0';
    return(dup);
}
