/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:49:12 by ixu               #+#    #+#             */
/*   Updated: 2024/02/17 17:09:27 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_dprintf.h"

int	print_s_only(char *str, t_dprintf data)
{
	if (str == NULL)
		return (write(data.fd, "(null)", 6));
	return (write(data.fd, str, ft_strlen(str)));
}

// print string up to len characters

static int	print_str_up_to_len(char *str, int len, t_dprintf data)
{
	if (str == NULL)
		return (print_str_up_to_len("(null)", len, data));
	return (write(data.fd, str, len));
}

/* print string up to len characters, 
left flag, width, precision are taken care of */

static int	print_string(char *str, t_dprintf data, int *count, int print_len)
{
	int	count_temp;

	if (data.left)
	{
		count_temp = print_str_up_to_len(str, print_len, data);
		if (count_temp == -1)
			return (-1);
		*count += count_temp;
	}
	if (data.width - print_len > 0)
	{
		count_temp = print_width(data, data.width - print_len);
		if (count_temp == -1)
			return (-1);
		*count += count_temp;
	}
	if (!data.left)
	{
		count_temp = print_str_up_to_len(str, print_len, data);
		if (count_temp == -1)
			return (-1);
		*count += count_temp;
	}
	return (0);
}

/*
	if (flags.prec >= 0 && flags.prec < strlen): // print_len = prec
		if (left): str->width
		if (!left): width->str
	else // print_len = strlen
		if (left): str->width
		if (!left): width->str
 */

int	print_str(char *str, t_dprintf data)
{
	int	count;
	int	count_temp;
	int	strlen;

	count = 0;
	if (str == NULL)
		strlen = 6;
	else
		strlen = (int)ft_strlen(str);
	if (data.prec >= 0 && data.prec < strlen)
	{
		count_temp = print_string(str, data, &count, data.prec);
		if (count_temp == -1)
			return (-1);
	}
	else
	{
		count_temp = print_string(str, data, &count, strlen);
		if (count_temp == -1)
			return (-1);
	}
	return (count);
}
