#include "../minishell.h"

int	fork1(t_data *data)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		panic(ERR_FORK, data, 1);
	return (pid);
}

int	run_redir(t_cmd *cmd, t_data *data)
{
	t_redircmd	*rcmd;
	int			process;
	int			fd;

	rcmd = (t_redircmd *)cmd;
	process = data->proc;
	if (data->under_pipe)
	{
		while (1)
		{
			if (make_filename(rcmd, data))
			{
				ft_dprintf(2, "run_redir: malloc or multiple filename.\n");
				return (1);
			}
			int	fd = open(rcmd->file, rcmd->mode, 0644);
			if (fd == -1)
				return (panic(rcmd->file, data, 1));
			if (dup2(fd, rcmd->fd) == -1)
				return (panic(ERR_DUP2, data, 1));
			if (close(fd) == -1)
				return (panic(ERR_CLOSE, data, 1));
			if (rcmd->cmd != NULL && rcmd->cmd->type == REDIR)
				rcmd = (t_redircmd *)rcmd->cmd;
			else
				break ;
		}
	}
	if (!data->under_pipe)
	{
		while (1)
		{
			if (make_filename(rcmd, data))
			{
				ft_dprintf(2, "run_redir: malloc or multiple filename.\n");
				if (!data->under_pipe)
				{
					if (dup2(data->fd_stdin, 0) == -1)
						return (panic(ERR_DUP2, data, 1));
					if (dup2(data->fd_stdout, 1) == -1)
						return (panic(ERR_DUP2, data, 1));
				}
				return (1);
			}
		//	printf("run_redir: file=->%s<-\n", rcmd->file);
		/*
			if (rcmd->mode == -1) // define a macro?
			{
				get_input(data, rcmd->file); // rcmd->file: delimiter
				if (data->status == 1)
				{
					if (process == CHILD_PROC)
						free_n_exit(data, 1);
					return (1);
				}
				fd = open(".heredoc", O_RDONLY);
				if (fd == -1)
					return (panic(".heredoc", data, 1));
			}
			else
		*/
			fd = open(rcmd->file, rcmd->mode, 0644);
			if (fd == -1)
			{
				// printf("paaaniiic\n");
				// maybe, something wrong here. check `<infile <missingfile >tempfile.txt
				// dprintf(2, "debug\n");
				if (dup2(data->fd_stdin, 0) == -1)
					return (panic(ERR_DUP2, data, 1));
				if (dup2(data->fd_stdout, 1) == -1)
					return (panic(ERR_DUP2, data, 1));
				return (panic(rcmd->file, data, 1));
				// dprintf(2, "debug1\n");
			}
			if (dup2(fd, rcmd->fd) == -1)
				return (panic(ERR_DUP2, data, 1));
			data->under_redir = 1;
			if (close(fd) == -1)
				return (panic("ERR_CLOSE?", data, 1));
			if (rcmd->cmd != NULL && rcmd->cmd->type == REDIR)
				rcmd = (t_redircmd *)rcmd->cmd;
			else
				break ;
		}
		// dprintf(2, "rcmd->file-%s\n", rcmd->file);
	}
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
	// if (data->under_pipe)
	// 	data->proc = CHILD_PROC;
	// else
	// data->proc = PARENT_PROC;
	// dprintf(2, "left of && is running...\n");
	runcmd(lcmd->left, data);
	// dprintf(2, "status: %d\n", data->status);
	if (data->status == 0)
	{
		// dprintf(2, "right of && is running...\n");
		// if (data->under_pipe)
		// 	data->proc = CHILD_PROC;
		// else
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
		// dprintf(2, "pipe flag: %d\n", data->under_pipe);
		if (close(pipe_fd[0]) == -1)
			panic("ERR_CLOSE0", data, 1); // recover error msg
		if (dup2(pipe_fd[1], 1) == -1)
			panic(ERR_DUP2, data, 1);
		if (close(pipe_fd[1]) == -1)
			panic("ERR_CLOSE1", data, 1);
		// t_redircmd	*rcmd;
		// if (pcmd->left->type == REDIR)
		// {
			// rcmd = (t_redircmd *)pcmd->left;
			// while (1)
			// {
			// 	if (make_filename(rcmd, data))
			// 	{
			// 		ft_dprintf(2, "run_redir: malloc or multiple filename.\n");
			// 		return (1);
			// 	}
			// 	int	fd = open(rcmd->file, rcmd->mode, 0644);
			// 	if (fd == -1)
			// 		return (panic(rcmd->file, data, 1));
			// 	if (dup2(fd, rcmd->fd) == -1)
			// 		return (panic(ERR_DUP2, data, 1));
			// 	if (close(fd) == -1)
			// 		return (panic(ERR_CLOSE, data, 1));
			// 	if (rcmd->cmd != NULL && rcmd->cmd->type == REDIR)
			// 		rcmd = (t_redircmd *)rcmd->cmd;
			// 	else
			// 		break ;
			// }
		// }
		runcmd(pcmd->left, data);
	}
	pid2 = fork1(data);
	if (pid2 == -1)
		return (1);
	if (pid2 == 0)
	{	
		// dprintf(2, "2nd child pipe flag: %d\n", data->under_pipe);
		data->proc = CHILD_PROC;
		if (close(pipe_fd[1]) == -1)
			panic("ERR_CLOSE2", data, 1);
		if (dup2(pipe_fd[0], 0) == -1)
			panic(ERR_DUP2, data, 1);
		if (close(pipe_fd[0]) == -1)
			panic("ERR_CLOSE3", data, 1);
		// t_redircmd	*rcmd;
		// if (pcmd->right->type == REDIR)
		// {
		// 	rcmd = (t_redircmd *)pcmd->right;
		// 	while (1)
		// 	{
		// 		if (make_filename(rcmd, data))
		// 		{
		// 			ft_dprintf(2, "run_redir: malloc or multiple filename.\n");
		// 			return (1);
		// 		}
		// 		int	fd = open(rcmd->file, rcmd->mode, 0644);
		// 		if (fd == -1)
		// 			return (panic(rcmd->file, data, 1));
		// 		if (dup2(fd, rcmd->fd) == -1)
		// 			return (panic(ERR_DUP2, data, 1));
		// 		if (close(fd) == -1)
		// 			return (panic(ERR_CLOSE, data, 1));
		// 		if (rcmd->cmd != NULL && rcmd->cmd->type == REDIR)
		// 			rcmd = (t_redircmd *)rcmd->cmd;
		// 		else
		// 			break ;
		// 	}	
		// }
		runcmd(pcmd->right, data);
	}
	if (ignore_signals() == 1)
		return (panic(ERR_SIGACTION, data, 1));
	if (close(pipe_fd[0]) == -1)
		return (panic("ERR_CLOSE4", data, 1));
	if (close(pipe_fd[1]) == -1)
		return (panic("ERR_CLOSE5", data, 1));
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
