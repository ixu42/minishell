/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:59:06 by ixu               #+#    #+#             */
/*   Updated: 2023/11/07 14:15:01 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

// converting char to int, meanwhile checking overflow

static int	convert(int is_neg, long *result, const char *str, int i)
{
	int	status;

	status = 1;
	if (is_neg == 0)
	{
		if (*result > LONG_MAX / 10)
			status = -1;
		*result = *result * 10;
		if (*result > LONG_MAX - (str[i] - '0'))
			status = -1;
		*result = *result + (str[i] - '0');
	}
	else
	{
		if (*result < LONG_MIN / 10)
			status = 0;
		*result = *result * 10;
		if (*result < LONG_MIN + (str[i] - '0'))
			status = 0;
		*result = *result - (str[i] - '0');
	}
	return (status);
}

// moving pointer of the string to the char that represents digit

static int	advance_to_digit(int *is_neg, const char **str)
{
	int	status;

	status = 1;
	while ((*str)[0] == ' '
		|| ((*str)[0] >= 9 && (*str)[0] <= 13))
		(*str)++;
	if (ft_isdigit((*str)[0]) == 0 && ft_isdigit((*str)[1]) == 0)
		status = 0;
	if ((*str)[0] == '-' && ft_isdigit((*str)[1]) == 1)
		*is_neg = 1;
	if ((*str)[0] == '-' || (*str)[0] == '+')
		(*str)++;
	return (status);
}

int	ft_atoi(const char *str)
{
	long	result;
	int		i;
	int		is_neg;
	int		status;

	is_neg = 0;
	if (str == NULL || ft_strlen(str) == 0)
		return (0);
	status = advance_to_digit(&is_neg, &str);
	if (status == 0)
		return (0);
	result = 0;
	i = 0;
	while (ft_isdigit(str[i]))
	{
		status = convert(is_neg, &result, str, i);
		if (status == -1 || status == 0)
			return (status);
		i++;
	}
	return ((int)result);
}
