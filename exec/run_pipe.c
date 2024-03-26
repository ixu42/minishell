/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 09:53:17 by ixu               #+#    #+#             */
/*   Updated: 2024/03/25 20:46:56 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	run_child(t_pipecmd *pcmd, t_data *data, int fd[2], int left_child)
{
	data->proc = CHILD_PROC;
	if (left_child)
	{
		if (close(fd[0]) == -1)
			panic(ERR_CLOSE, data, 1);
		if (dup2(fd[1], 1) == -1)
			panic(ERR_DUP2, data, 1);
		if (close(fd[1]) == -1)
			panic(ERR_CLOSE, data, 1);
		runcmd(pcmd->left, data);
	}
	else
	{
		if (close(fd[1]) == -1)
			panic(ERR_CLOSE, data, 1);
		if (dup2(fd[0], 0) == -1)
			panic(ERR_DUP2, data, 1);
		if (close(fd[0]) == -1)
			panic(ERR_CLOSE, data, 1);
		runcmd(pcmd->right, data);
	}
}

static int	fork_n_run(t_pipecmd *pcmd, pid_t pid[2], int fd[2], t_data *data)
{
	pid[0] = fork1(data);
	if (pid[0] == -1)
		return (1);
	if (pid[0] == 0)
		run_child(pcmd, data, fd, 1);
	pid[1] = fork1(data);
	if (pid[1] == -1)
		return (1);
	if (pid[1] == 0)
		run_child(pcmd, data, fd, 0);
	return (0);
}

static void	pass_status_to_parent_node(int status, int process, t_data *data)
{
	if (WIFEXITED(status))
	{
		if (process == PARENT_PROC)
			data->status = WEXITSTATUS(status);
		else
			free_n_exit(data, WEXITSTATUS(status));
	}
	if (WIFSIGNALED(status))
	{
		handle_signal_output(WTERMSIG(status));
		if (process == PARENT_PROC)
			data->status = 128 + WTERMSIG(status);
		else
			free_n_exit(data, 128 + WTERMSIG(status));
	}
}

int	run_pipe(t_cmd *cmd, t_data *data)
{
	t_pipecmd	*pcmd;
	int			process;
	int			pipe_fd[2];
	pid_t		pid[2];
	int			status;

	pcmd = (t_pipecmd *)cmd;
	process = data->proc;
	data->under_pipe = 1;
	if (pipe(pipe_fd) == -1)
		return (panic(ERR_PIPE, data, 1));
	if (fork_n_run(pcmd, pid, pipe_fd, data) == 1)
		return (1);
	if (ignore_signals() == 1)
		return (panic(ERR_SIGACTION, data, 1));
	if (close(pipe_fd[0]) == -1)
		return (panic(ERR_CLOSE, data, 1));
	if (close(pipe_fd[1]) == -1)
		return (panic(ERR_CLOSE, data, 1));
	if (waitpid(pid[0], NULL, 0) == -1)
		return (panic(ERR_WAITPID, data, 1));
	if (waitpid(pid[1], &status, 0) == -1)
		return (panic(ERR_WAITPID, data, 1));
	pass_status_to_parent_node(status, process, data);
	return (0);
}
