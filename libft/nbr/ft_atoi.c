/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:59:06 by ixu               #+#    #+#             */
/*   Updated: 2024/02/17 16:57:00 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

static int	convert(int is_neg, long *ptr_to_result, const char *str, int i)
{
	int	return_flag;

	return_flag = 1;
	if (is_neg == 0)
	{
		if (*ptr_to_result > LONG_MAX / 10)
			return_flag = -1;
		*ptr_to_result = *ptr_to_result * 10;
		if (*ptr_to_result > LONG_MAX - (str[i] - '0'))
			return_flag = -1;
		*ptr_to_result = *ptr_to_result + (str[i] - '0');
	}
	else
	{
		if (*ptr_to_result < LONG_MIN / 10)
			return_flag = 0;
		*ptr_to_result = *ptr_to_result * 10;
		if (*ptr_to_result < LONG_MIN + (str[i] - '0'))
			return_flag = 0;
		*ptr_to_result = *ptr_to_result - (str[i] - '0');
	}
	return (return_flag);
}

static int	skip_spaces(int *ptr_to_is_neg, const char **ptr_to_str)
{
	int	return_flag;

	return_flag = 1;
	while (*(*ptr_to_str) == ' '
		|| (*(*ptr_to_str) >= 9 && *(*ptr_to_str) <= 13))
		(*ptr_to_str)++;
	if (ft_isdigit((*ptr_to_str)[0]) == 0 && ft_isdigit((*ptr_to_str)[1]) == 0)
		return_flag = 0;
	if ((*ptr_to_str)[0] == '-' && ft_isdigit((*ptr_to_str)[1]) == 1)
		*ptr_to_is_neg = 1;
	if ((*ptr_to_str)[0] == '-' || (*ptr_to_str)[0] == '+')
		(*ptr_to_str)++;
	return (return_flag);
}

int	ft_atoi(const char *str)
{
	long	result;
	int		i;
	int		is_neg;
	int		return_flag;

	is_neg = 0;
	if (ft_strlen(str) == 0)
		return (0);
	return_flag = skip_spaces(&is_neg, &str);
	if (return_flag == 0)
		return (0);
	result = 0;
	i = 0;
	while (ft_isdigit(str[i]))
	{
		return_flag = convert(is_neg, &result, str, i);
		if (return_flag == -1 || return_flag == 0)
			return (return_flag);
		i++;
	}
	return ((int)result);
}
