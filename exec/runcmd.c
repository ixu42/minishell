/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:13:23 by ixu               #+#    #+#             */
/*   Updated: 2024/03/21 23:20:47 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* the parameter process to indicate if runcmd 
is executed in child process or in parent process
in child process -> exit with status code; 
in parent process -> status code stored in data and return */

void	runcmd(t_cmd *cmd, t_data *data)
{
	if (cmd->type == EXEC)
	{
		// ------ debug ------
		// dprintf(2, "exec\n");
		// -------------------
		run_exec(cmd, data);
		if (data->status)
			return ;
	}
	else if (cmd->type == REDIR)
	{
		// ------ debug ------
		// dprintf(2, "redir\n");
		// -------------------
		run_redir(cmd, data);
		if (data->status)
			return ;
	}
	else if (cmd->type == AND_CMD)
	{
		// ------ debug ------
		// dprintf(2, "&& operator\n");
		// -------------------
		run_and(cmd, data);
	}
	else if (cmd->type == OR_CMD)
	{
		// ------ debug ------
		// dprintf(2, "|| operator\n");
		// -------------------
		run_or(cmd, data);
	}
	else if (cmd->type == PIPE)
	{
		// ------ debug ------
		// dprintf(2, "pipe\n");
		// -------------------
		run_pipe(cmd, data);
		if (data->status)
			return ;
	}
}
