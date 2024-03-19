/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:45:33 by ixu               #+#    #+#             */
/*   Updated: 2024/03/19 13:49:04 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

static int	convert(int is_neg, long *result, char *str)
{
	if (is_neg == 0)
	{
		if (*result > LONG_MAX / 10)
			return (1);
		*result = *result * 10;
		if (*result > LONG_MAX - (*str - '0'))
			return (1);
		*result = *result + (*str - '0');
	}
	else
	{
		if (*result < LONG_MIN / 10)
			return (2);
		*result = *result * 10;
		if (*result < LONG_MIN + (*str - '0'))
			return (2);
		*result = *result - (*str - '0');
	}
	return (0);
}

static int	skip_spaces(int *is_neg, char **str)
{
	int	status;

	status = 0;
	while (*(*str) == ' '
		|| (*(*str) >= 9 && *(*str) <= 13))
		(*str)++;
	if (ft_isdigit((*str)[0]) == 0 && ft_isdigit((*str)[1]) == 0)
		status = 1;
	if ((*str)[0] == '-' && ft_isdigit((*str)[1]) == 1)
		*is_neg = 1;
	if ((*str)[0] == '-' || (*str)[0] == '+')
		(*str)++;
	return (status);
}

static int	checks_before_digits(char **str, int *err, int *is_neg)
{
	int	status;

	if (ft_strlen(*str) == 0)
	{
		*err = 1;
		return (1);
	}
	status = skip_spaces(is_neg, str);
	if (status == 1)
	{
		*err = 1;
		return (1);
	}
	return (0);
}

/* the flag err checks if (1) the characters preceding 
the first sequence of numbers (if any) are not valid, 
(2) overflow has occurred, or (3) the first sequence of 
numbers (if any) does not end with '\0' */

long	ft_strtol(char *str, int *err)
{
	long	result;
	int		is_neg;
	int		status;

	*err = 0;
	is_neg = 0;
	if (checks_before_digits(&str, err, &is_neg) == 1)
		return (0);
	result = 0;
	while (ft_isdigit(*str))
	{
		status = convert(is_neg, &result, str);
		if (status == 1 || status == 2)
			*err = 1;
		if (status == 1)
			return (LONG_MAX);
		if (status == 2)
			return (LONG_MIN);
		str++;
	}
	if (*str != '\0')
		*err = 1;
	return (result);
}
