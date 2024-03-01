/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:45:07 by ixu               #+#    #+#             */
/*   Updated: 2024/02/25 19:11:36 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_dprintf.h"

// width and all flags are intialized to 0 (false), and precision to -1

t_dprintf	initialize_data(int fd)
{
	t_dprintf	data;

	data.fd = fd;
	data.left = 0;
	data.zero = 0;
	data.hash = 0;
	data.space = 0;
	data.sign = 0;
	data.width = 0;
	data.prec = -1;
	data.star = 0;
	return (data);
}

void	set_left_flag(t_dprintf **data)
{
	(*data)->left = 1;
	(*data)->zero = 0;
}

void	eval_star(t_dprintf **data, va_list ap)
{
	(*data)->star = 1;
	(*data)->width = va_arg(ap, int);
	if ((*data)->width < 0)
	{
		(*data)->left = 1;
		(*data)->width *= -1;
	}
}

void	get_width(char c, t_dprintf **data)
{
	(*data)->width = (*data)->width * 10 + (c - '0');
}

int	get_prec(const char *format, int i, t_dprintf **data, va_list ap)
{
	int	j;

	(*data)->zero = 0;
	j = i + 1;
	if (format[j] == '*')
	{
		(*data)->prec = va_arg(ap, int);
		return (j++);
	}
	(*data)->prec = 0;
	while (ft_isdigit(format[j]))
	{
		(*data)->prec = (*data)->prec * 10 + (format[j] - '0');
		j++;
	}
	return (j);
}
