/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 21:04:40 by ixu               #+#    #+#             */
/*   Updated: 2024/01/11 21:04:40 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*last_node;

	if (lst == NULL)
		return (NULL);
	last_node = lst;
	while (last_node->next != NULL)
	{
		last_node = last_node->next;
	}
	return (last_node);
}
