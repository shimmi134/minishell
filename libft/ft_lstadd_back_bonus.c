/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 13:38:30 by shimi-be          #+#    #+#             */
/*   Updated: 2024/09/28 14:41:44 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*aux_lst;

	aux_lst = *lst;
	if (!aux_lst)
	{
		*lst = new;
		return ;
	}
	while (aux_lst -> next != NULL)
		aux_lst = aux_lst -> next;
	aux_lst -> next = new;
}
/*
#include <stdio.h>

int	main(void)
{
	t_list *lst;
	t_list *head;

	lst = ft_lstnew("HOla");
	head = NULL;
	ft_lstadd_back(&head, lst);
	while (head)
	{
		printf("%p\n", head);
		head = head -> next;
	}

}*/
