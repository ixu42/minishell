/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:13:56 by ixu               #+#    #+#             */
/*   Updated: 2024/03/21 16:20:48 by ixu              ###   ########.fr       */
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

/* int	recover_stdin_n_stdout(t_data *data)
{
	if (!data->under_pipe && data->under_redir)
	{
		if (dup2(data->fd_stdin, 0) == -1)
			return (panic(ERR_DUP2, data, 1));
		if (dup2(data->fd_stdout, 1) == -1)
			return (panic(ERR_DUP2, data, 1));
	}
} */

int	run_redir(t_cmd *cmd, t_data *data)
{
	t_redircmd	*rcmd;
	int			process;
	int			fd;

	rcmd = (t_redircmd *)cmd;
	data->under_redir = 1;
	if (make_filename(rcmd, data))
	{
		if (!data->under_pipe)
		{
			if (dup2(data->fd_stdin, 0) == -1)
				return (panic(ERR_DUP2, data, 1));
			if (dup2(data->fd_stdout, 1) == -1)
				return (panic(ERR_DUP2, data, 1));
		}
		return (panic(NULL, data, 1));
	}
	fd = open(rcmd->file, rcmd->mode, 0644);
	if (fd == -1)
	{
		if (!data->under_pipe)
		{
			if (dup2(data->fd_stdin, 0) == -1)
				return (panic(ERR_DUP2, data, 1));
			if (dup2(data->fd_stdout, 1) == -1)
				return (panic(ERR_DUP2, data, 1));
		}
		return (panic(rcmd->file, data, 1));
	}
	if (dup2(fd, rcmd->fd) == -1)
		return (panic(ERR_DUP2, data, 1));
	if (close(fd) == -1)
		return (panic(ERR_CLOSE, data, 1));
	process = data->proc;
	data->proc = PARENT_PROC;
	runcmd(rcmd->cmd, data);
	if (process == CHILD_PROC)
		free_n_exit(data, data->status);
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
		data->proc = PARENT_PROC;
		runcmd(lcmd->right, data);
	}
	if (process == CHILD_PROC)
		free_n_exit(data, data->status);
}

int	run_pipe(t_cmd *cmd, t_data *data)
{
	t_pipecmd	*pcmd;
	int			process;
	int			pipe_fd[2];
	pid_t		pid1;
	pid_t		pid2;
	int			status;

	pcmd = (t_pipecmd *)cmd;
	process = data->proc;
	data->under_pipe = 1;
	if (pipe(pipe_fd) == -1)
		return (panic(ERR_PIPE, data, 1));
	pid1 = fork1(data);
	if (pid1 == -1)
		return (1);
	if (pid1 == 0)
	{
		data->proc = CHILD_PROC;
		if (close(pipe_fd[0]) == -1)
			panic(ERR_CLOSE, data, 1);
		if (dup2(pipe_fd[1], 1) == -1)
			panic(ERR_DUP2, data, 1);
		if (close(pipe_fd[1]) == -1)
			panic(ERR_CLOSE, data, 1);
		runcmd(pcmd->left, data);
	}
	pid2 = fork1(data);
	if (pid2 == -1)
		return (1);
	if (pid2 == 0)
	{	
		data->proc = CHILD_PROC;
		if (close(pipe_fd[1]) == -1)
			panic(ERR_CLOSE, data, 1);
		if (dup2(pipe_fd[0], 0) == -1)
			panic(ERR_DUP2, data, 1);
		if (close(pipe_fd[0]) == -1)
			panic(ERR_CLOSE, data, 1);
		runcmd(pcmd->right, data);
	}
	if (ignore_signals() == 1)
		return (panic(ERR_SIGACTION, data, 1));
	if (close(pipe_fd[0]) == -1)
		return (panic(ERR_CLOSE, data, 1));
	if (close(pipe_fd[1]) == -1)
		return (panic(ERR_CLOSE, data, 1));
	if (waitpid(pid1, NULL, 0) == -1)
		return (panic(ERR_WAITPID, data, 1));
	if (waitpid(pid2, &status, 0) == -1)
		return (panic(ERR_WAITPID, data, 1));
	if (WIFEXITED(status) && process == PARENT_PROC)
		data->status = WEXITSTATUS(status);
	if (WIFSIGNALED(status) && process == PARENT_PROC)
		data->status = 128 + WTERMSIG(status);
	if (WIFEXITED(status) && process == CHILD_PROC)
		free_n_exit(data, WEXITSTATUS(status));
	if (WIFSIGNALED(status) && process == CHILD_PROC)
		free_n_exit(data, 128 + WTERMSIG(status));
	return (0);
}
