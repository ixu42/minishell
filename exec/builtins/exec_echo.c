/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:14:54 by ixu               #+#    #+#             */
/*   Updated: 2024/03/24 13:16:59 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	print_args(char **argv, int i)
{
	while (argv[i] != NULL)
	{
		if (write(1, argv[i], ft_strlen(argv[i])) == -1)
		{
			perror(PMT_ERR_WRITE);
			return (1);
		}
		if (argv[++i] != NULL)
		{
			if (write(1, " ", 1) == -1)
			{
				perror(PMT_ERR_WRITE);
				return (1);
			}
		}
	}
	return (0);
}

int	exec_echo(char **argv)
{
	int	add_new_line;
	int	start_index;
	int	i;

	add_new_line = 0;
	start_index = 1;
	if (argv[1] != NULL && ft_strcmp(argv[1], "-n") == 0)
		start_index++;
	else
		add_new_line = 1;
	i = start_index;
	if (print_args(argv, i) == 1)
		return (1);
	if (add_new_line)
	{
		if (write(1, "\n", 1) == -1)
		{
			perror(PMT_ERR_WRITE);
			return (1);
		}
	}
	return (0);
}
