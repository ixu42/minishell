/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:48:57 by ixu               #+#    #+#             */
/*   Updated: 2024/02/17 17:09:21 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_dprintf.h"

static int	print_hex_only(unsigned int n, int is_X, t_dprintf data)
{
	int		count;
	char	*symbols;

	symbols = "0123456789abcdef";
	if (is_X)
		symbols = "0123456789ABCDEF";
	if (data.prec == 0 && n == 0)
		return (0);
	if (n >= 16)
	{
		count = print_hex_only(n / 16, is_X, data);
		if (count == -1)
			return (-1);
		return (count + print_c_only(symbols[n % 16], data));
	}
	else
		return (print_c_only(symbols[n], data));
}

// print first digits then width

static int	print_digit_width(unsigned int n, int is_X, t_dprintf data, int len)
{
	int	count;
	int	count_temp;

	count = 0;
	count_temp = print_hash(n, is_X, data);
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	count_temp = print_prec(data, data.prec - digits_len((long)n, 16, data));
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	count_temp = print_hex_only(n, is_X, data);
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	count_temp = print_width(data, data.width - len);
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	return (count);
}

// print width and hash based on flag 0

static int	print_hash_width(unsigned int n, int is_X, t_dprintf data, int len)
{
	int	count;
	int	count_temp;

	count = 0;
	if (data.zero)
	{
		count_temp = print_hash(n, is_X, data);
		if (count_temp == -1)
			return (-1);
		count += count_temp;
	}
	count_temp = print_width(data, data.width - len);
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	if (!data.zero)
	{
		count_temp = print_hash(n, is_X, data);
		if (count_temp == -1)
			return (-1);
		count += count_temp;
	}
	return (count);
}

// print first width then digits

static int	print_width_digit(unsigned int n, int is_X, t_dprintf data, int len)
{
	int	count;
	int	count_temp;

	count = 0;
	count_temp = print_hash_width(n, is_X, data, len);
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	count_temp = print_prec(data, data.prec - digits_len((long)n, 16, data));
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	count_temp = print_hex_only(n, is_X, data);
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	return (count);
}

/* 
	if (left)
		prec->num->width(' ')
	if (!left)
		hash->width(0)->num || 
		width(' ')->hash->prec->num 
*/

int	print_hex(unsigned int n, int is_X, t_dprintf data)
{
	int	len;
	int	count;
	int	count_temp;

	count = 0;
	len = digits_len((long)n, 16, data);
	if (data.hash && n != 0)
		len += 2;
	if (data.prec - digits_len((long)n, 16, data) > 0)
		len += data.prec - digits_len((long)n, 16, data);
	if (data.left)
	{
		count_temp = print_digit_width(n, is_X, data, len);
		if (count_temp == -1)
			return (-1);
		count += count_temp;
	}
	if (!data.left)
	{
		count_temp = print_width_digit(n, is_X, data, len);
		if (count_temp == -1)
			return (-1);
		count += count_temp;
	}
	return (count);
}
