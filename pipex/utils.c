/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 16:56:20 by ixu               #+#    #+#             */
/*   Updated: 2024/01/16 16:56:20 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	data_init(t_data **data, char **envp)
{
	char	**arr;

	arr = NULL;
	if (*envp == NULL)
	{
		arr = ft_split("/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin", ':');
		if (arr == NULL)
			print_error_partial_free(ERR_MALLOC, *data);
		(*data)->env_paths = arr;
	}
	else
		(*data)->env_paths = get_env_paths(envp, *data);
	(*data)->cmd_arr = NULL;
	(*data)->cmd_path = NULL;
}

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

void	close_fds(t_data *data, int fd1, int fd2)
{
	if (close(fd1) == -1)
	{
		perror("pipex: close error");
		free_struct(data);
		exit(EXIT_FAILURE);
	}
	if (close(fd2) == -1)
	{
		perror("pipex: close error");
		free_struct(data);
		exit(EXIT_FAILURE);
	}
}
