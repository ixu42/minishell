/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 17:49:47 by ixu               #+#    #+#             */
/*   Updated: 2024/02/17 16:57:05 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

static int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	else
		return (n);
}

static int	ft_power(int x, int y)
{
	int	i;
	int	result;

	i = 0;
	result = 1;
	while (i < y)
	{
		result *= x;
		i++;
	}
	return (result);
}

static int	ft_count_len(int n)
{
	int	len;
	int	i;

	len = 0;
	i = 10;
	if (n == -2147483648)
		return (11);
	if (n < 0)
		len++;
	while (i >= 2)
	{
		if (ft_abs(n) >= ft_power(10, i - 1))
			return (len += i);
		i--;
	}
	return (len += 1);
}

static void	write_number(char *str, int len, int n)
{
	int		digit1_index;
	char	*min_int;

	digit1_index = 0;
	min_int = "-2147483648";
	if (n == -2147483648)
	{
		while (*min_int)
			*(str++) = *(min_int++);
		return ;
	}
	if (n < 0)
	{
		str[0] = '-';
		n = -n;
		digit1_index = 1;
	}
	while (len > digit1_index)
	{
		str[len - 1] = n % 10 + '0';
		len--;
		n = n / 10;
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;

	len = ft_count_len(n);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);
	write_number(str, len, n);
	str[len] = '\0';
	return (str);
}
