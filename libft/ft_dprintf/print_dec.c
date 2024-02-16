/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_dec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 13:17:25 by ixu               #+#    #+#             */
/*   Updated: 2024/02/17 17:09:18 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_dprintf.h"

// print decimal only

static int	print_dec_only(long n, t_dprintf data)
{
	int		count;

	if (data.prec == 0 && n == 0)
		return (0);
	if (n < 0)
		n *= -1;
	if (n >= 10)
	{
		count = print_dec_only(n / 10, data);
		if (count == -1)
			return (-1);
		return (count + print_c_only(n % 10 + '0', data));
	}
	else
		return (print_c_only(n % 10 + '0', data));
}

// print first digits then width

static int	print_digit_width(long n, t_dprintf data, int len)
{
	int	count;
	int	count_temp;

	count = 0;
	count_temp = print_sign(data, ft_is_neg(n));
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	count_temp = print_prec(data, data.prec - digits_len(n, 10, data));
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	count_temp = print_dec_only(n, data);
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	count_temp = print_width(data, data.width - len);
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	return (count);
}

// print width with/without flag 0

static int	print_width_sign(long n, t_dprintf data, int len)
{
	int	count;
	int	count_temp;

	count = 0;
	if (data.zero)
	{
		count_temp = print_sign(data, ft_is_neg(n));
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
		count_temp = print_sign(data, ft_is_neg(n));
		if (count_temp == -1)
			return (-1);
		count += count_temp;
	}
	return (count);
}

// print first width then digits

static int	print_width_digit(long n, t_dprintf data, int len)
{
	int	count;
	int	count_temp;

	count = 0;
	count_temp = print_width_sign(n, data, len);
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	count_temp = print_prec(data, data.prec - digits_len(n, 10, data));
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	count_temp = print_dec_only(n, data);
	if (count_temp == -1)
		return (-1);
	count += count_temp;
	return (count);
}

/* print decimal and manage flags and precision
	%[flags][width][.precision]conversion
	flags = "-0 +"
	0 flag is ignored, if
		1. a precision is given with an integer conversion (d, i, o, u, x, and X)
		2. - flags is given
	+ overrides ' ' if both are used

	0 padded precision depends on length of digits (excl. sign)
	+|' '|precision affects length of full digits (excl. width padding)
	-|width|0 pad spaces/zeros

	when 0 is printed with precision 0, the output is empty.

	if (left)
		sign->prec->num(pos part)->width(' ')
	if (!left)
		sign->width(0)->num(pos part) || 
		width(' ')->sign->prec->num(pos part)	
*/

int	print_dec(long n, t_dprintf data)
{
	int	len;
	int	count;
	int	count_temp;

	count = 0;
	len = digits_len(n, 10, data);
	if (data.sign || data.space || n < 0)
		len += 1;
	if (data.prec - digits_len(n, 10, data) > 0)
		len += data.prec - digits_len(n, 10, data);
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
