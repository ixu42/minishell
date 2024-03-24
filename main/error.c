/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:14:24 by ixu               #+#    #+#             */
/*   Updated: 2024/03/22 21:01:32 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	validate_args(int argc)
{
	if (argc != 1)
	{
		if (ft_putendl_fd(ERR_ARGS, 2) == -1)
			print_error_n_exit(ERR_WRITE);
		if (ft_putendl_fd(USAGE, 1) == -1)
			print_error_n_exit(ERR_WRITE);
		exit(1);
	}
}

/* buf is considered valid if (1) it is not empty string,
and (2) it contains at least one character other than white spaces */

int	is_valid_buf(char *buf)
{
	int	i;
	int	j;
	int	is_white_space;

	if (ft_strlen(buf) == 0)
		return (0);
	i = -1;
	while (buf[++i] != '\0')
	{
		j = -1;
		is_white_space = 0;
		while (WHITESPACE[++j] != '\0')
		{
			if (buf[i] == WHITESPACE[j])
			{
				is_white_space = 1;
				break ;
			}
		}
		if (!is_white_space)
			return (1);
	}
	return (0);
}

/* used in main and data init. (1) print error 
message, (2) exit */

void	print_error_n_exit(char *err_msg)
{
	if (ft_putstr_fd("minishell: ", 2) == -1)
	{
		perror("minishell: write error");
		exit(1);
	}
	perror(err_msg);
	exit(1);
}
