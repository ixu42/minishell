/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 21:14:22 by ixu               #+#    #+#             */
/*   Updated: 2024/03/24 17:47:31 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/get_next_line.h"

int	has_newline_char(t_lst *list)
{
	t_lst	*last_node;
	int		i;

	if (list == NULL)
		return (0);
	last_node = list;
	while (last_node->next != NULL)
		last_node = last_node->next;
	i = 0;
	while (last_node->str_buf[i] != '\0')
	{
		if (last_node->str_buf[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

int	ft_lst_append(t_lst **lst, char *buf)
{
	t_lst	*ptr;
	t_lst	*new_node;

	new_node = (t_lst *)malloc(sizeof(t_lst));
	if (new_node == NULL)
		return (-2);
	new_node->next = NULL;
	new_node->str_buf = buf;
	if (*lst == NULL)
	{
		*lst = new_node;
		return (1);
	}
	ptr = *lst;
	while (ptr->next != NULL)
		ptr = ptr->next;
	ptr->next = new_node;
	return (1);
}

void	free_list(t_lst **list)
{
	t_lst	*temp;

	temp = *list;
	while (*list != NULL)
	{
		*list = (*list)->next;
		free(temp->str_buf);
		free(temp);
		temp = *list;
	}
}

int	ft_str_len(t_lst *list)
{
	int		i;
	int		str_len;
	t_lst	*temp;

	str_len = 0;
	temp = list;
	while (temp != NULL)
	{
		i = 0;
		while (temp->str_buf[i] != '\0')
		{
			if (temp->str_buf[i] == '\n')
			{
				str_len++;
				break ;
			}
			str_len++;
			i++;
		}
		temp = temp->next;
	}
	return (str_len);
}

char	*ft_str_cpy(char *str, t_lst *list)
{
	int		i;
	int		j;
	t_lst	*temp;

	j = 0;
	temp = list;
	while (temp != NULL)
	{
		i = 0;
		while (temp->str_buf[i] != '\0')
		{
			if (temp->str_buf[i] == '\n')
			{
				str[j++] = temp->str_buf[i];
				break ;
			}
			str[j] = temp->str_buf[i];
			i++;
			j++;
		}
		temp = temp->next;
	}
	str[j] = '\0';
	return (str);
}
