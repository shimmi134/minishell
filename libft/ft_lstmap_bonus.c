/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 13:39:56 by shimi-be          #+#    #+#             */
/*   Updated: 2024/09/25 14:13:16 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*aux_lst;
	t_list	*aux_lst2;
	void	*content;

	aux_lst2 = NULL;
	while (lst)
	{
		content = f(lst -> content);
		if (!content)
		{
			ft_lstclear(&aux_lst2, del);
			return (NULL);
		}
		aux_lst = ft_lstnew(content);
		if (!aux_lst)
		{
			free(content);
			ft_lstclear(&aux_lst2, del);
			return (NULL);
		}
		ft_lstadd_back(&aux_lst2, aux_lst);
		lst = lst -> next;
	}
	return (aux_lst2);
}
