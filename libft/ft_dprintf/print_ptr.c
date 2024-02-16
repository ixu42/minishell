/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:49:05 by ixu               #+#    #+#             */
/*   Updated: 2024/02/17 17:09:24 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_dprintf.h"

static int	print_address(unsigned long n, t_dprintf data)
{
	char	*symbols;
	int		count;

	symbols = "0123456789abcdef";
	if (n >= 16)
	{
		count = print_address(n / 16, data);
		if (count == -1)
			return (-1);
		return (count + print_c_only(symbols[n % 16], data));
	}
	else
		return (print_c_only(symbols[n], data));
}

static int	address_len(unsigned long n)
{
	int	len;

	len = 2;
	if (n >= 16)
	{
		len = address_len(n / 16);
		return (len + 1);
	}
	else
		return (len + 1);
	return (len);
}

// print the pointer

static int	print_pointer(unsigned long n, t_dprintf data)
{
	int	count;
	int	count_temp;

	count = 0;
	if (n == 0)
		return (print_s_only(PRINT_NULL, data));
	count_temp = print_s_only("0x", data);
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	count_temp = print_address(n, data);
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	return (count);
}

// print the pointer and take care of the flags

int	print_ptr(unsigned long n, t_dprintf data)
{
	int	count;
	int	count_temp;

	count = 0;
	if (data.left)
	{
		count_temp = print_pointer(n, data);
		if (count_temp == -1)
			return (-1);
		count += count_temp;
	}
	count_temp = print_width(data, data.width - address_len(n));
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	if (!data.left)
	{
		count_temp = print_pointer(n, data);
		if (count_temp == -1)
			return (-1);
		count += count_temp;
	}
	return (count);
}
