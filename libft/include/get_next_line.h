/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 21:16:11 by ixu               #+#    #+#             */
/*   Updated: 2024/01/11 21:16:11 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# define MAX_FD 256
# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

typedef struct s_lst
{
	char			*str_buf;
	struct s_lst	*next;
}	t_lst;

char	*get_next_line(int fd, int *malloc_err, int *read_err);
int		has_newline_char(t_lst *list);
int		ft_lst_append(t_lst **lst, char *buf, int *malloc_err);
void	free_list(t_lst **list);
int		ft_str_len(t_lst *list);
char	*ft_str_cpy(char *str, t_lst *list);

#endif