/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 13:38:35 by shimi-be          #+#    #+#             */
/*   Updated: 2024/09/27 13:25:33 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*aux_lst;
	t_list	*aux2_lst;

	aux2_lst = *lst;
	while (aux2_lst)
	{
		aux_lst = aux2_lst -> next;
		del(aux2_lst -> content);
		free(aux2_lst);
		aux2_lst = aux_lst;
	}
	*lst = NULL;
}
