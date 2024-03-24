/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 19:47:19 by ixu               #+#    #+#             */
/*   Updated: 2024/03/24 13:17:09 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	redirect_io(t_redircmd *rcmd, t_data *data)
{
	int	fd;

	fd = open(rcmd->file, rcmd->mode, 0644);
	if (fd == -1)
	{
		if (restore_stdin_n_stdout(data) == 1)
			return (1);
		return (panic(rcmd->file, data, 1));
	}
	if (dup2(fd, rcmd->fd) == -1)
		return (panic(ERR_DUP2, data, 1));
	if (close(fd) == -1)
		return (panic(ERR_CLOSE, data, 1));
	return (0);
}

int	run_redir(t_cmd *cmd, t_data *data)
{
	t_redircmd	*rcmd;
	int			process;

	rcmd = (t_redircmd *)cmd;
	data->under_redir = 1;
	if (make_filename(rcmd, data))
	{
		if (restore_stdin_n_stdout(data) == 1)
			return (1);
		return (panic(NULL, data, 1));
	}
	if (redirect_io(rcmd, data) == 1)
		return (1);
	process = data->proc;
	data->proc = PARENT_PROC;
	runcmd(rcmd->cmd, data);
	if (process == CHILD_PROC)
		free_n_exit(data, data->status);
	return (0);
}
