/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:46:21 by ixu               #+#    #+#             */
/*   Updated: 2024/02/17 17:09:06 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_dprintf.h"

static int	print_arg(const char *format, int i, t_dprintf data, va_list ap)
{
	int	count;

	count = 0;
	if (format[i] == 'c')
		count = print_char(va_arg(ap, int), data);
	else if (format[i] == 's')
		count = print_str(va_arg(ap, char *), data);
	else if (format[i] == 'd' || format[i] == 'i')
		count = print_dec((long)va_arg(ap, int), data);
	else if (format[i] == 'u')
		count = print_udec((unsigned int)va_arg(ap, int), data);
	else if (format[i] == 'x')
		count = print_hex((unsigned int)va_arg(ap, int), 0, data);
	else if (format[i] == 'X')
		count = print_hex((unsigned int)va_arg(ap, int), 1, data);
	else if (format[i] == 'p')
		count = print_ptr((unsigned long)va_arg(ap, void *), data);
	else if (format[i] == '%')
		count = print_char('%', data);
	else
		count = -1;
	return (count);
}

static int	parse_flags(const char *format, int i, t_dprintf *data, va_list ap)
{
	while (!is_specifier(format[++i]))
	{
		if (format[i] == '-')
			set_left_flag(&data);
		else if (format[i] == '0' && data->width == 0 && data->left == 0)
			data->zero = 1;
		else if (format[i] == '#')
			data->hash = 1;
		else if (format[i] == ' ' && data->sign == 0)
			data->space = 1;
		else if (format[i] == '+')
			data->sign = 1;
		else if (format[i] == '*')
			eval_star(&data, ap);
		else if (ft_isdigit(format[i]))
			get_width(format[i], &data);
		else if (format[i] == '.')
			i = get_prec(format, i, &data, ap);
		else
			return (i);
		if (is_specifier(format[i]))
			break ;
	}
	return (i);
}

static int	eval_format(const char *format, va_list ap, t_dprintf data)
{
	int	i;
	int	count;
	int	count_temp;

	i = -1;
	count = 0;
	while (format[++i])
	{
		if (format[i] == '%' && format[i + 1] != '\0')
		{
			i = parse_flags(format, i, &data, ap);
			count_temp = print_arg(format, i, data, ap);
			data = data_init(data.fd);
		}
		else if (format[i] == '%' && format[i + 1] == '\0')
			return (-1);
		else
			count_temp = print_c_only(format[i], data);
		if (count_temp == -1)
			return (-1);
		count += count_temp;
	}
	return (count);
}

/* 
The overall syntax of a conversion specification is: 
	%[flags][width][.precision]conversion
	flags = "-0# +"
*/

int	ft_dprintf(int fd, const char *format, ...)
{
	va_list		ap;
	t_dprintf	data;
	int			count;

	data = data_init(fd);
	va_start(ap, format);
	count = eval_format(format, ap, data);
	va_end(ap);
	return (count);
}
