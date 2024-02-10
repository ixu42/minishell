/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 09:51:35 by ixu               #+#    #+#             */
/*   Updated: 2024/02/09 19:26:08 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(char *err_msg)
{
	if (ft_putstr_fd("minishell: ", STDERR_FILENO) == -1)
	{
		perror("minishell: write error");
		exit(EXIT_FAILURE);
	}
	perror(err_msg);
	exit(EXIT_FAILURE);
}

void	validate_args(int argc)
{
	if (argc != 1)
	{
		if (ft_putendl_fd(ERR_ARGS, STDERR_FILENO) == -1)
			print_error(ERR_WRITE);
		if (ft_putendl_fd(USAGE, STDOUT_FILENO) == -1)
			print_error(ERR_WRITE);
		exit(EXIT_FAILURE);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*buf; // could be in a struct

	(void)argv;
	(void)envp;
	validate_args(argc);
	buf = readline(">> ");
	while (buf != NULL) 
	{
		if (ft_strlen(buf) > 0)
			add_history(buf);
		printf("[%s]\n", buf); // replace this line with parsing func
		if (ft_strcmp(buf, "exit") == 0) // exit could be a boolean in a struct (parsing part to detect exit)
		{
			free(buf);
			break ;
		}
		free(buf); // where to free buf in different cases?
		buf = readline(">> ");
	}
	rl_clear_history();
	exit(EXIT_SUCCESS);
}
