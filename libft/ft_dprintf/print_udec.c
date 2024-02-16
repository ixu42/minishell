/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_udec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 21:39:45 by ixu               #+#    #+#             */
/*   Updated: 2024/02/17 17:09:30 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_dprintf.h"

static int	print_udec_only(unsigned int n, t_dprintf data)
{
	int		count;

	if (data.prec == 0 && n == 0)
		return (0);
	if (n >= 10)
	{
		count = print_udec_only(n / 10, data);
		if (count == -1)
			return (-1);
		return (count + print_c_only(n % 10 + '0', data));
	}
	else
		return (print_c_only(n % 10 + '0', data));
}

// print first digits then width

static int	print_digit_width(unsigned int n, t_dprintf data, int len)
{
	int	count;
	int	count_temp;

	count = 0;
	count_temp = print_prec(data, data.prec - digits_len((long)n, 10, data));
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	count_temp = print_udec_only(n, data);
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	count_temp = print_width(data, data.width - len);
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	return (count);
}

// print first width then digits

static int	print_width_digit(unsigned int n, t_dprintf data, int len)
{
	int	count;
	int	count_temp;

	count = 0;
	count_temp = print_width(data, data.width - len);
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	count_temp = print_prec(data, data.prec - digits_len((long)n, 10, data));
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	count_temp = print_udec_only(n, data);
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	return (count);
}

/* 
	if (left)
		prec->num->width(' ')
	if (!left)
		width(0)->num || 
		width(' ')->prec->num 
*/

int	print_udec(unsigned int n, t_dprintf data)
{
	int	len;
	int	count;
	int	count_temp;

	count = 0;
	len = digits_len((long)n, 10, data);
	if (data.prec - digits_len((long)n, 10, data) > 0)
		len += data.prec - digits_len((long)n, 10, data);
	if (data.left)
	{
		count_temp = print_digit_width(n, data, len);
		if (count_temp == -1)
			return (-1);
		count += count_temp;
	}
	if (!data.left)
	{
		count_temp = print_width_digit(n, data, len);
		if (count_temp == -1)
			return (-1);
		count += count_temp;
	}
	return (count);
}
