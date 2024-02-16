/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_chars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:48:35 by ixu               #+#    #+#             */
/*   Updated: 2024/02/17 17:09:15 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_dprintf.h"

int	print_c_only(int c, t_dprintf data)
{
	return (write(data.fd, &c, 1));
}

/* 
	if (left): char->width(' ')
	if (!left): width(' ')->char
*/

int	print_char(int c, t_dprintf data)
{
	int	count;
	int	count_temp;

	count = 0;
	if (data.left)
	{
		count_temp = print_c_only(c, data);
		if (count_temp == -1)
			return (-1);
		count += count_temp;
	}
	count_temp = print_width(data, data.width - 1);
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	if (!data.left)
	{
		count_temp = print_c_only(c, data);
		if (count_temp == -1)
			return (-1);
		count += count_temp;
	}
	return (count);
}
