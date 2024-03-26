/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:13:23 by ixu               #+#    #+#             */
/*   Updated: 2024/03/26 11:48:37 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	fork1(t_data *data)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		panic(ERR_FORK, data, 1);
	return (pid);
}

int	restore_stdin_n_stdout(t_data *data)
{
	if (!data->under_pipe && data->under_redir)
	{
		if (dup2(data->fd_stdin, 0) == -1)
			return (panic(ERR_DUP2, data, 1));
		if (dup2(data->fd_stdout, 1) == -1)
			return (panic(ERR_DUP2, data, 1));
	}
	return (0);
}

void	run_and(t_cmd *cmd, t_data *data)
{
	t_listcmd	*lcmd;
	int			process;

	lcmd = (t_listcmd *)cmd;
	process = data->proc;
	data->proc = PARENT_PROC;
	runcmd(lcmd->left, data);
	if (data->status == 0)
	{
		if (set_default_signals() == 1)
		{
			panic(ERR_SIGACTION, data, 1);
			return ;
		}
		data->proc = PARENT_PROC;
		runcmd(lcmd->right, data);
	}
	if (process == CHILD_PROC)
		free_n_exit(data, data->status);
}

void	run_or(t_cmd *cmd, t_data *data)
{
	t_listcmd	*lcmd;
	int			process;

	lcmd = (t_listcmd *)cmd;
	process = data->proc;
	data->proc = PARENT_PROC;
	runcmd(lcmd->left, data);
	if (data->status != 0)
	{
		if (set_default_signals() == 1)
		{
			panic(ERR_SIGACTION, data, 1);
			return ;
		}
		data->proc = PARENT_PROC;
		runcmd(lcmd->right, data);
	}
	if (process == CHILD_PROC)
		free_n_exit(data, data->status);
}

/* the parameter process to indicate if runcmd 
is executed in child process or in parent process
in child process -> exit with status code; 
in parent process -> status code stored in data and return */

void	runcmd(t_cmd *cmd, t_data *data)
{
	if (cmd->type == EXEC)
	{
		run_exec(cmd, data);
		if (data->status)
			return ;
	}
	else if (cmd->type == REDIR)
	{
		run_redir(cmd, data);
		if (data->status)
			return ;
	}
	else if (cmd->type == AND_CMD)
		run_and(cmd, data);
	else if (cmd->type == OR_CMD)
		run_or(cmd, data);
	else if (cmd->type == PIPE)
	{
		run_pipe(cmd, data);
		if (data->status)
			return ;
	}
}
