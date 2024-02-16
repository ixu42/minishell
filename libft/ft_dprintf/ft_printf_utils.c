/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 13:39:48 by ixu               #+#    #+#             */
/*   Updated: 2024/02/17 17:09:11 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_dprintf.h"

// check if the passed char is a conversion specifier

int	is_specifier(char c)
{
	if (c == 'c' || c == 's' || c == 'p' || c == 'd' || c == 'i'
		|| c == 'u' || c == 'x' || c == 'X' || c == '%')
		return (1);
	return (0);
}

int	digits_len(long n, int base, t_dprintf data)
{
	int	len;

	len = 0;
	if (data.prec == 0 && n == 0)
		return (0);
	if (n < 0)
		n *= -1;
	if (n >= base)
	{
		len = digits_len(n / base, base, data);
		return (len + 1);
	}
	else
		return (len + 1);
	return (len);
}

int	ft_is_neg(long n)
{
	if (n < 0)
		return (1);
	else
		return (0);
}
