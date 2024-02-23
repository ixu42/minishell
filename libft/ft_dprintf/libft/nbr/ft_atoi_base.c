/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 11:19:27 by ixu               #+#    #+#             */
/*   Updated: 2024/01/27 11:19:27 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_isspace(int c)
{
	if (c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r' || c == ' ')
		return (1);
	return (0);
}

static void	advance_to_digit(const char *str, int *i, int *sign)
{
	while (ft_isspace(str[*i]))
		(*i)++;
	*sign = 1;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			*sign *= -1;
		(*i)++;
	}
}

static int	base_digit(char c, int base)
{
	char	*symbols_lower;
	char	*symbols_upper;
	int		i;

	symbols_lower = "0123456789abcdef";
	symbols_upper = "0123456789ABCDEF";
	i = 0;
	while (i < base)
	{
		if (c == symbols_lower[i] || c == symbols_upper[i])
			return (i);
		i++;
	}
	return (-1);
}

static int	convert(int sign, long *result, char c, int base)
{
	int	status;

	status = 1;
	if (sign == 1)
	{
		if (*result > LONG_MAX / base)
			status = -1;
		*result = *result * base;
		if (*result > LONG_MAX - base_digit(c, base))
			status = -1;
		*result = *result + base_digit(c, base);
	}
	else
	{
		if (*result < LONG_MIN / base)
			status = 0;
		*result = *result * base;
		if (*result < LONG_MIN + base_digit(c, base))
			status = 0;
		*result = *result - base_digit(c, base);
	}
	return (status);
}

int	ft_atoi_base(const char *str, int base)
{
	int			i;
	int			sign;
	long		result;
	int			status;

	if (str == NULL)
		return (0);
	i = 0;
	advance_to_digit(str, &i, &sign);
	result = 0;
	while (base_digit(str[i], base) != -1)
	{
		status = convert(sign, &result, str[i], base);
		if (status == -1 || status == 0)
			return (status);
		i++;
	}
	return ((int)result);
}
