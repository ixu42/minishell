/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:49:24 by ixu               #+#    #+#             */
/*   Updated: 2024/02/17 17:09:32 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_dprintf.h"

// pad width with zero or space up to len characters

int	print_width(t_dprintf data, int print_len)
{
	int	count;
	int	count_temp;

	count = 0;
	while (print_len > 0)
	{
		if (data.zero)
			count_temp = print_c_only('0', data);
		else
			count_temp = print_c_only(' ', data);
		if (count_temp == -1)
			return (-1);
		count += count_temp;
		print_len--;
	}
	return (count);
}

static int	print_space(t_dprintf data, int is_neg)
{
	int	count;
	int	count_temp;

	count = 0;
	if (data.space && !is_neg)
	{
		count_temp = print_c_only(' ', data);
		if (count_temp == -1)
			return (-1);
		count += count_temp;
	}
	return (count);
}

int	print_sign(t_dprintf data, int is_neg)
{
	int	count;
	int	count_temp;

	count = 0;
	if (data.sign && !is_neg)
	{
		count_temp = print_c_only('+', data);
		if (count_temp == -1)
			return (-1);
		count += count_temp;
	}
	if (is_neg)
	{
		count_temp = print_c_only('-', data);
		if (count_temp == -1)
			return (-1);
		count += count_temp;
	}
	count_temp = print_space(data, is_neg);
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	return (count);
}

int	print_prec(t_dprintf data, int print_len)
{
	int	count;
	int	count_temp;

	count = 0;
	if (data.prec != -1)
	{
		while (print_len > 0)
		{
			count_temp = print_c_only('0', data);
			if (count_temp == -1)
				return (-1);
			count += count_temp;
			print_len--;
		}
	}
	return (count);
}

int	print_hash(unsigned int n, int is_X, t_dprintf data)
{
	int	count;
	int	count_temp;

	count = 0;
	count_temp = 0;
	if (data.hash)
	{
		if (is_X && n != 0)
			count_temp = print_s_only("0X", data);
		else if (!is_X && n != 0)
			count_temp = print_s_only("0x", data);
		if (count_temp == -1)
			return (-1);
		count += count_temp;
	}
	return (count);
}
