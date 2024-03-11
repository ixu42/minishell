/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 21:04:58 by ixu               #+#    #+#             */
/*   Updated: 2024/01/11 21:04:58 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*head;
	t_list	*new;
	void	*get_content;

	if (lst == NULL)
		return (NULL);
	head = NULL;
	while (lst)
	{
		get_content = f(lst->content);
		new = ft_lstnew(get_content);
		if (new == NULL)
		{
			if (get_content != lst->content)
				del(get_content);
			if (head != NULL)
				ft_lstclear(&head, del);
			return (NULL);
		}
		ft_lstadd_back(&head, new);
		lst = lst->next;
	}
	return (head);
}
