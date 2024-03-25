/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arraylist.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 11:05:41 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/25 13:14:26 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	size_t	copy_size;
	void	*new_ptr;

	if (!ptr)
		return (malloc(new_size));
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	ft_memset(new_ptr, 0, new_size);
	if (old_size < new_size)
		copy_size = old_size;
	else
		copy_size = new_size;
	ft_memcpy(new_ptr, ptr, copy_size);
	free(ptr);
	return (new_ptr);
}

t_arrlist	*create_arrlist(void)
{
	t_arrlist	*list;

	list = (t_arrlist *)malloc(sizeof(*list));
	if (!list)
		return (NULL);
	list->data = (char **)malloc(INITIAL_CAPACITY * sizeof(char *));
	if (list->data == NULL)
	{
		free(list);
		return (NULL);
	}
	ft_memset(list->data, 0, INITIAL_CAPACITY * sizeof(char *));
	list->size = 0;
	list->capacity = INITIAL_CAPACITY;
	return (list);
}

int	add_string_arrlist(t_arrlist *list, const char *str)
{
	size_t	capacity;

	if (list->size >= list->capacity - 1)
	{
		capacity = list->capacity * GROWTH_FACTOR;
		list->data = (char **)ft_realloc(list->data, \
			list->capacity * sizeof(char *), capacity * sizeof(char *));
		list->capacity = capacity;
		if (list->data == NULL)
			return (1);
	}
	list->data[list->size] = ft_strdup(str);
	if (list->data[list->size] == NULL)
		return (1);
	list->size++;
	return (0);
}

void	free_arrlist(t_arrlist *list)
{
	size_t	i;

	i = 0;
	while (i < list->size)
	{
		if (list->data[i])
		{
			free(list->data[i]);
			list->data[i] = NULL;
		}
		i++;
	}
	free(list->data);
	list->data = NULL;
	free(list);
}
