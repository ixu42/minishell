/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 21:11:26 by ixu               #+#    #+#             */
/*   Updated: 2024/01/14 23:07:00 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

// handling error and no freeing

void	print_error_no_free(char *name)
{
	if (ft_putstr_fd("pipex: ", STDERR_FILENO) == -1)
	{
		perror("pipex: write error");
		exit(EXIT_FAILURE);
	}
	perror(name);
	exit(EXIT_FAILURE);
}

// handling error and only freeing data pointer

void	print_error_partial_free(char *name, t_data *data)
{
	if (ft_putstr_fd("pipex: ", STDERR_FILENO) == -1)
	{
		perror("pipex: write error");
		free(data);
		data = NULL;
		exit(EXIT_FAILURE);
	}
	perror(name);
	free(data);
	data = NULL;
	exit(EXIT_FAILURE);
}

void	print_error(char *name, t_data *data, int exit_code)
{
	if (ft_putstr_fd("pipex: ", STDERR_FILENO) == -1)
	{
		perror("pipex: write error");
		free_struct(data);
		exit(EXIT_FAILURE);
	}
	if (exit_code == EXIT_CMD_NOT_FOUND)
	{
		if (ft_putstr_fd(name, STDERR_FILENO) == -1)
		{
			perror(ERR_WRITE);
			free_struct(data);
			exit(EXIT_FAILURE);
		}
		if (ft_putendl_fd(ERR_CMD, STDERR_FILENO) == -1)
		{
			perror(ERR_WRITE);
			free_struct(data);
			exit(EXIT_FAILURE);
		}
	}
	else
		perror(name);
	free_struct(data);
	exit(exit_code);
}

void	exit_handler(char **argv, int argc)
{
	if (ft_putendl_fd(ERR_INPUT, STDERR_FILENO) == -1)
	{
		perror("pipex: write error");
		exit(EXIT_FAILURE);
	}
	if (argc > 1 && ft_strcmp(argv[1], "here_doc") == 0)
	{
		if (ft_putendl_fd(EX_HEREDOC_ARGS, STDOUT_FILENO) == -1)
		{
			perror("pipex: write error");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (ft_putendl_fd(EX_ARGS, STDOUT_FILENO) == -1)
		{
			perror("pipex: write error");
			exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_FAILURE);
}
