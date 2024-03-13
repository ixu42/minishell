#include "../minishell.h"

void	run_redir(t_cmd *cmd, t_data *data)
{
	t_redircmd	*rcmd;
	int			process;
	int			fd;

	rcmd = (t_redircmd *)cmd;
	process = data->proc;
	if (rcmd->mode == -1) // define a macro?
	{
		get_input(data, rcmd->file); // rcmd->file: delimiter
		if (data->status == 1)
		{
			if (process == CHILD_PROC)
				free_n_exit(data, 1);
			return ;
		}
		fd = open(".heredoc", O_RDONLY);
	}
	else
		fd = open(rcmd->file, rcmd->mode, 0644);
	if (fd == -1)
	{
		panic(ERR_OPEN, data, 1);
		return ;
	}
	if (dup2(fd, rcmd->fd) == -1)
	{
		panic(ERR_DUP2, data, 1);
		return ;
	}
	if (close(fd) == -1)
	{
		panic(ERR_CLOSE, data, 1);
		return ;
	}
	data->proc = PARENT_PROC;
	runcmd(rcmd->cmd, data);
	if (process == CHILD_PROC)
		free_n_exit(data, data->status);
}

void	run_and(t_cmd *cmd, t_data *data)
{
	t_listcmd	*lcmd;

	lcmd = (t_listcmd *)cmd;
	data->proc = PARENT_PROC;
	runcmd(lcmd->left, data);
	if (data->status == 0)
	{
		data->proc = PARENT_PROC;
		runcmd(lcmd->right, data);
	}
}

void	run_or(t_cmd *cmd, t_data *data)
{
	t_listcmd	*lcmd;

	lcmd = (t_listcmd *)cmd;
	data->proc = PARENT_PROC;
	runcmd(lcmd->left, data);
	if (data->status != 0)
	{
		data->proc = PARENT_PROC;
		runcmd(lcmd->right, data);
	}	
}

void	run_pipe(t_cmd *cmd, t_data *data)
{
	t_pipecmd	*pcmd;
	int			process;
	int			pipe_fd[2];
	int			pid1;
	int			pid2;
	int			status;

	pcmd = (t_pipecmd *)cmd;
	process = data->proc;
	if (pipe(pipe_fd) == -1)
	{
		panic(ERR_PIPE, data, 1);
		return ;
	}
	pid1 = fork1(data);
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
	if (close(pipe_fd[0]) == -1)
	{
		panic(ERR_CLOSE, data, 1);
		return ;
	}
	if (close(pipe_fd[1]) == -1)
	{
		panic(ERR_CLOSE, data, 1);
		return ;
	}
	if (waitpid(pid1, NULL, 0) == -1)
	{
		panic(ERR_WAITPID, data, 1);
		return ;
	}
	if (waitpid(pid2, &status, 0) == -1)
	{
		panic(ERR_WAITPID, data, 1);
		return ;
	}
	if (WIFEXITED(status) && process == PARENT_PROC)
		data->status = WEXITSTATUS(status);
	if (WIFEXITED(status) && process == CHILD_PROC)
		free_n_exit(data, WEXITSTATUS(status));
}