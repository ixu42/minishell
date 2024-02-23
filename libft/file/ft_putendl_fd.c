/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:48:50 by ixu               #+#    #+#             */
/*   Updated: 2024/02/17 16:53:44 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_putendl_fd(char *s, int fd)
{
	if (s == NULL)
	{
		if (ft_putstr_fd(PRINT_NULL, fd) == -1)
			return (-1);
		return (0);
	}
	if (write(fd, s, ft_strlen(s)) == -1)
		return (-1);
	if (write(fd, "\n", sizeof(char)) == -1)
		return (-1);
	return (0);
}
