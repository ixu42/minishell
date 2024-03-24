/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 20:38:31 by ixu               #+#    #+#             */
/*   Updated: 2024/03/24 16:17:06 by ixu              ###   ########.fr       */
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
		{
			perror(PMT_ERR_WRITE);
			return (handle_exit_or_return(data, 1));
		}
		perror(msg);
	}
	return (handle_exit_or_return(data, status_code));
}

int	panic_is_a_dir(char *msg, t_data *data)
{
	char	*full_msg;

	if (msg != NULL)
	{
		full_msg = join_msgs(PMT, msg, ": is a directory\n");
		if (full_msg == NULL)
			return (handle_exit_or_return(data, 1));
		if (ft_dprintf(2, "%s", full_msg) == -1)
		{
			free(full_msg);
			perror(PMT_ERR_WRITE);
			return (handle_exit_or_return(data, 1));
		}
		free(full_msg);
	}
	return (handle_exit_or_return(data, 126));
}

// error handling in case of command not found

int	panic_cmd_not_found(char *cmd, t_data *data)
{
	char	*full_msg;

	if (cmd != NULL)
	{
		full_msg = join_msgs(PMT, cmd, ": command not found\n");
		if (full_msg == NULL)
			return (handle_exit_or_return(data, 1));
		if (ft_dprintf(2, "%s", full_msg) == -1)
		{
			free(full_msg);
			perror(PMT_ERR_WRITE);
			return (handle_exit_or_return(data, 1));
		}
		free(full_msg);
	}
	return (handle_exit_or_return(data, 127));
}

int	perror_n_return(char *msg, int return_value)
{
	perror(msg);
	return (return_value);
}
