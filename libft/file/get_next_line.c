/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 21:13:29 by ixu               #+#    #+#             */
/*   Updated: 2024/03/24 21:08:38 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/get_next_line.h"

static void	create_list(t_lst **list, int fd, int *malloc_err, int *read_err)
{
	int		chars_read;
	char	*buf;

	while (!has_newline_char(list[fd]))
	{
		buf = (char *)malloc(BUFFER_SIZE + 1);
		if (buf == NULL)
		{
			*malloc_err = 1;
			return ;
		}
		chars_read = read(fd, buf, BUFFER_SIZE);
		if (chars_read == -1 || (chars_read == 0 && list[fd] == NULL))
		{
			free(buf);
			*read_err = -chars_read;
			return ;
		}
		buf[chars_read] = '\0';
		if (ft_lst_append(&list[fd], buf, malloc_err) == 1)
		{
			free(buf);
			return ;
		}
	}
}

static char	*list_to_str(t_lst *list, int *malloc_err)
{
	char	*str;

	str = (char *)malloc((ft_str_len(list) + 1) * sizeof(char));
	if (str == NULL)
	{
		*malloc_err = 1;
		return (NULL);
	}
	str = ft_str_cpy(str, list);
	return (str);
}

static void	fill_new_node(t_lst *list, t_lst **new_node, char **buf)
{
	t_lst	*last_node;
	int		i;
	int		j;

	last_node = list;
	while (last_node->next != NULL)
		last_node = last_node->next;
	i = 0;
	j = 0;
	while (last_node->str_buf[i] != '\0' && last_node->str_buf[i] != '\n')
		i++;
	while (last_node->str_buf[i] && last_node->str_buf[++i])
		(*buf)[j++] = last_node->str_buf[i];
	(*buf)[j] = '\0';
	(*new_node)->str_buf = *buf;
	(*new_node)->next = NULL;
}

static void	update_list(t_lst **list, int *malloc_err)
{
	char	*buf;
	t_lst	*new_node;

	buf = (char *)malloc(BUFFER_SIZE + 1);
	new_node = (t_lst *)malloc(sizeof(t_lst));
	if (buf == NULL || new_node == NULL)
	{
		free(new_node);
		free(buf);
		free_list(list);
		*malloc_err = 1;
		return ;
	}
	fill_new_node(*list, &new_node, &buf);
	free_list(list);
	*list = NULL;
	if (new_node->str_buf[0])
		*list = new_node;
	else
	{
		free(buf);
		free(new_node);
	}
}

// An array of linked list is used to handle multiple fd.

char	*get_next_line(int fd, int *malloc_err, int *read_err)
{
	static t_lst	*list[MAX_FD + 1];
	char			*next_line;

	if (fd < 0 || fd > MAX_FD || BUFFER_SIZE <= 0)
		return (NULL);
	create_list(list, fd, malloc_err, read_err);
	if (*malloc_err || *read_err || !list[fd])
	{
		free_list(&list[fd]);
		return (NULL);
	}
	next_line = list_to_str(list[fd], malloc_err);
	if (*malloc_err)
	{
		free_list(&list[fd]);
		return (NULL);
	}
	update_list(&list[fd], malloc_err);
	if (*malloc_err)
	{
		free(next_line);
		return (NULL);
	}
	return (next_line);
}
