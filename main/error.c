/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:14:24 by ixu               #+#    #+#             */
/*   Updated: 2024/03/22 09:52:20 by ixu              ###   ########.fr       */
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
		exit(EXIT_FAILURE);
	}
}

// (1) print error message, (2) exit

void	print_error_n_exit(char *err_msg)
{
	if (ft_putstr_fd("minishell: ", 2) == -1)
	{
		perror("minishell: write error");
		exit(EXIT_FAILURE);
	}
	perror(err_msg);
	exit(EXIT_FAILURE);
}

// (1) print error message, (2) set error flag to 1, and (3) return NULL

t_env	*error_handler(char *err_msg, int *err_flag)
{
	if (ft_putstr_fd("minishell: ", 2) == -1)
	{
		perror(PMT_ERR_WRITE);
		*err_flag = 1;
		return (NULL);
	}
	perror(err_msg);
	*err_flag = 1;
	return (NULL);
}

/* (1) print error message (if msg is NULL, no error msg 
will be printed), (2) if in child process, free all heap 
allocated memory and exit with status code; if in parent 
process, data->status is set to status code */

int	panic(char *msg, t_data *data, int status_code)
{
	if (msg != NULL)
	{
		if (ft_dprintf(2, "%s", PMT) == -1)
			perror(PMT_ERR_WRITE);
		perror(msg);
	}
	if (data->proc == CHILD_PROC)
	{
		free_data(data);
		// free tree nodes?
		exit(status_code);
	}
	data->status = status_code;
	return (1);
}

int	panic_is_a_dir(char *msg, t_data *data)
{
	if (ft_dprintf(2, "%s%s: is a directory\n", PMT, msg) == -1)
		perror(PMT_ERR_WRITE);
	if (data->proc == CHILD_PROC)
	{
		free_data(data);
		// free tree nodes?
		exit(126);
	}
	data->status = 126;
	return (1);
}

// error handling in case of command not found

int	panic_cmd_not_found(char *msg, t_data *data)
{
	if (ft_dprintf(2, "%s%s: command not found\n", PMT, msg) == -1)
		perror(PMT_ERR_WRITE);
	if (data->proc == CHILD_PROC)
	{
		free(data->cmd_path);
		free_data(data);
		// free tree nodes?
		exit(127);
	}
	data->status = 127;
	return (1);
}

// used during the execution

void	free_n_exit(t_data *data, int status_code)
{
	free_data(data);
	// free AST in child proccesses?
	exit(status_code);
}
