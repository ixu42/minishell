/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:47:55 by ixu               #+#    #+#             */
/*   Updated: 2024/02/17 16:49:35 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DPRINTF_H
# define FT_DPRINTF_H
# include <unistd.h>
# include <stdarg.h>
# include <stdlib.h>
# include "../ft_dprintf/libft/libft.h"

typedef struct s_dprintf
{
	int	fd;
	int	left;
	int	zero;
	int	prec;
	int	width;
	int	star;
	int	hash;
	int	space;
	int	sign;
}	t_dprintf;

// ft_print() function
int			ft_dprintf(int fd, const char *format, ...);

// funcs for analyzing or modifying flags
t_dprintf	data_init(int fd);
void		set_left_flag(t_dprintf **data);
void		eval_star(t_dprintf **data, va_list ap);
void		get_width(char c, t_dprintf **data);
int			get_prec(const char *format, int i, t_dprintf **data, va_list ap);

// funcs for printing specifiers (csdiuxXp)
// c
int			print_c_only(int c, t_dprintf data);
int			print_char(int c, t_dprintf data);
// s
int			print_s_only(char *str, t_dprintf data);
int			print_str(char *str, t_dprintf data);
// d, i
int			print_dec(long n, t_dprintf data);
// u
int			print_udec(unsigned int n, t_dprintf data);
// x, X
int			print_hex(unsigned int n, int is_X, t_dprintf data);
// p
int			print_ptr(unsigned long int add, t_dprintf data);
// helper funcs for the prints
int			print_width(t_dprintf data, int len);
int			print_sign(t_dprintf data, int is_neg);
int			print_prec(t_dprintf data, int print_len);
int			print_hash(unsigned int n, int is_X, t_dprintf data);

// other helper funcs
int			is_specifier(char c);
int			digits_len(long n, int base, t_dprintf data);
int			ft_is_neg(long n);

#endif