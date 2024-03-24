/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 20:38:31 by ixu               #+#    #+#             */
/*   Updated: 2024/03/23 11:46:22 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* used in implementing env builtin. (1) print error message, 
(2) set error flag to 1, and (3) return NULL */

t_env	*error_handler(char *err_msg, int *err_flag)
{
	if (ft_putstr_fd(PMT, 2) == -1)
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
		free_arr(data->envp);
		free_data(data);
		data = NULL;
		// free tree nodes?
		exit(127);
	}
	data->status = 127;
	return (1);
}

int	perror_n_return(char *msg, int return_value)
{
	perror(msg);
	return (return_value);
}
