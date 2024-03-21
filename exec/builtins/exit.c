/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:16:12 by ixu               #+#    #+#             */
/*   Updated: 2024/03/21 23:20:48 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_valid(char *arg)
{
	long	num;
	int		err;

	num = ft_strtol(arg, &err);
	if (err == 1)
		return (0);
	return (1);
}

static void	perror_n_exit(char *msg, int exit_code)
{
	if (ft_dprintf(2, "%s\n", msg) == -1)
		perror_n_exit(PMT_ERR_WRITE, 1);
	exit(exit_code);
}

/* Exit the shell, returning a status of n to the shell’s parent. 
If n is omitted, the exit status is that of the last command executed */

int	exec_exit(char **argv) // clean everything before exit
{
	int	err;

	if (argv[1] == NULL)
		perror_n_exit("exit", 0);
	else if (argv[2] != NULL)
	{
		if (ft_dprintf(2, "%sexit: too many arguments\n", PMT) == -1)
			perror(PMT_ERR_WRITE);
		return (1);
	}
	else
	{
		if (!is_valid(argv[1]))
		{
			if (write(2, "exit\n", 5) == -1)
				perror_n_exit(PMT_ERR_WRITE, 1);
			if (ft_dprintf(2, "%sexit: %s: ", PMT, argv[1]) == -1)
				perror_n_exit(PMT_ERR_WRITE, 1);
			if (ft_dprintf(2, "numeric argument required\n") == -1)
				perror_n_exit(PMT_ERR_WRITE, 1);
			exit(255);
		}
		perror_n_exit("exit", (unsigned char)ft_strtol(argv[1], &err));
	}
	return (0);
}
