#include "../minishell.h"

int	fork1(t_data *data)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		panic(ERR_FORK, data, 1);
	return (pid);
}

int	run_exec(t_cmd *cmd, t_data *data)
{
	t_execcmd	*ecmd;
	pid_t		pid;
	int			status;

	ecmd = (t_execcmd *)cmd;
	make_argv(ecmd, data);
	// if (ecmd->argv == NULL || ecmd->argv[0] == NULL)
	// {
	// 	dprintf(2, "debug\n");
	// 	if (data->proc == CHILD_PROC)
	// 		free_n_exit(data, 0);
	// 	else
	// 	{
	// 		data->status = 0;
	// 		return (0);
	// 	}
	// }
	// ------ print out args ------
	//for (int i = 0; ecmd->argv[i] != NULL; i++)
	// 	dprintf(2, "ecmd->argv[%d]: %s\n", i, ecmd->argv[i]);
	// ----------------------------
	if (ecmd->argv != NULL && ecmd->argv[0] != NULL && is_builtin(ecmd->argv, &data))
	{
		// ------ debug ------
		// dprintf(2, "builtin\n");
		// -------------------
		data->status = run_builtin(ecmd->argv, data);
		if (dup2(data->fd_stdin, 0) == -1)
			return (panic(ERR_DUP2, data, 1));
		if (dup2(data->fd_stdout, 1) == -1)
			return (panic(ERR_DUP2, data, 1));
		if (data->proc == CHILD_PROC)
			free_n_exit(data, data->status);
	}
	else
	{
		// ------ debug ------
		// dprintf(2, "non-builtin\n");
		// -------------------
		if (data->proc == CHILD_PROC)
		{
			data->cmd_path = get_cmd_path(ecmd->argv, data); // free
			data->envp = copy_env_lst_to_arr(data->env_lst);
			// ------ print out arr ------
			// for (int n = 0; data->envp[n] != NULL; n++)
			// 	printf("%s\n", data->envp[n]);
			// ----------------------------
			execve(data->cmd_path, ecmd->argv, data->envp);
			if (ecmd->argv[0] == NULL)
				panic("", data, 127);
			panic(ecmd->argv[0], data, 127);
		}
		else
		{
			// signal(SIGINT, SIG_DFL);
			pid = fork1(data);
			if (pid == -1)
				return (1);
			if (pid == 0)
			{
				data->proc = CHILD_PROC;
				if (child_signal_handler() == 1)
					panic(ERR_SIGACTION, data, 1);
				data->cmd_path = get_cmd_path(ecmd->argv, data); // free
				// dprintf(2, "data->cmd_path: %s\n", data->cmd_path);
				// ------ print out list ------
				// t_env	*tmp = data->env_lst;
				// while (tmp != NULL)
				// {
				// 	printf("%s=%s\n", tmp->name, tmp->value);
				// 	tmp = tmp->next;
				// }
				// ----------------------------
				data->envp = copy_env_lst_to_arr(data->env_lst);
				// ------ print out arr ------
				// for (int n = 0; data->envp[n] != NULL; n++)
				// 	dprintf(2 ,"%s\n", data->envp[n]);
				// ----------------------------
				execve(data->cmd_path, ecmd->argv, data->envp);
				if (ecmd->argv[0] == NULL)
					panic("", data, 127);
				panic(ecmd->argv[0], data, 127);
			}
			if (parent_signal_handler() == 1)
				return (panic(ERR_SIGACTION, data, 1));
			if (dup2(data->fd_stdin, 0) == -1)
				return (panic(ERR_DUP2, data, 1));
			if (dup2(data->fd_stdout, 1) == -1)
				return (panic(ERR_DUP2, data, 1));
			if (waitpid(pid, &status, 0) == -1)
				return (panic(ERR_WAITPID, data, 1));
			if (WIFEXITED(status))
				data->status = WEXITSTATUS(status);
			if (WIFSIGNALED(status))
				data->status = 128 + WTERMSIG(status);
		}
	}
	return (0);
}

int	run_redir(t_cmd *cmd, t_data *data)
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
			return (1);
		}
		fd = open(".heredoc", O_RDONLY);
		if (fd == -1)
			return (panic(".heredoc", data, 1));
	}
	else
	{
		fd = open(rcmd->file, rcmd->mode, 0644);
		if (fd == -1)
			return (panic(rcmd->file, data, 1));
	}
	if (dup2(fd, rcmd->fd) == -1)
		return (panic(ERR_DUP2, data, 1));
	if (close(fd) == -1)
		return (panic(ERR_CLOSE, data, 1));
	data->proc = PARENT_PROC;
	runcmd(rcmd->cmd, data);
	if (process == CHILD_PROC)
		free_n_exit(data, data->status);
	return (0);
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
	if (pipe(pipe_fd) == -1)
		return (panic(ERR_PIPE, data, 1));
	pid1 = fork1(data);
	if (pid1 == -1)
		return (1);
	if (parent_signal_handler() == 1)
		return (panic(ERR_SIGACTION, data, 1));
	if (pid1 == 0)
	{
		data->proc = CHILD_PROC;
		if (child_signal_handler() == 1)
			panic(ERR_SIGACTION, data, 1);	
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
	if (parent_signal_handler() == 1)
		return (panic(ERR_SIGACTION, data, 1));
	if (pid2 == 0)
	{	data->proc = CHILD_PROC;
		if (child_signal_handler() == 1)
			panic(ERR_SIGACTION, data, 1);
		if (close(pipe_fd[1]) == -1)
			panic(ERR_CLOSE, data, 1);
		if (dup2(pipe_fd[0], 0) == -1)
			panic(ERR_DUP2, data, 1);
		if (close(pipe_fd[0]) == -1)
			panic(ERR_CLOSE, data, 1);
		runcmd(pcmd->right, data);
	}
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
