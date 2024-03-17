#include "../minishell.h"

int	fork1(t_data *data)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		panic(ERR_FORK, data, 1);
	return (pid);
}

void	run_exec(t_cmd *cmd, t_data *data)
{
	t_execcmd	*ecmd;
	int			pid;
	int			status;

	ecmd = (t_execcmd *)cmd;
	if (ecmd->argc == 0)
	{
		ft_dprintf(2, "run_exec: nothing to execute here: argc = 0!, status = 0\n");
	//	data->status = 0;
	}
	make_argv(ecmd, data);
	if (ecmd->argv == NULL || ecmd->argv[0] == NULL)
	{
		if (data->proc == CHILD_PROC)
			free_n_exit(data, 0);
		else
		{
			data->status = 0;
			return ;
		}
	}
	// ------ print out args ------
	//for (int i = 0; ecmd->argv[i] != NULL; i++)
	// 	dprintf(2, "ecmd->argv[%d]: %s\n", i, ecmd->argv[i]);
	// ----------------------------
	if (is_builtin(ecmd->argv, &data))
	{
		// ------ debug ------
		// dprintf(2, "builtin\n");
		// -------------------
		data->status = run_builtin(ecmd->argv, data);
		if (dup2(data->fd_stdin, 0) == -1)
		{
			panic(ERR_DUP2, data, 1);
			return ;
		}
		if (dup2(data->fd_stdout, 1) == -1)
		{
			panic(ERR_DUP2, data, 1);
			return ;
		}
		if (data->proc == CHILD_PROC)
			free_n_exit(data, data->status);
		// else
		// 	return ;
	}
	else
	{
		// ------ debug ------
		// dprintf(2, "non-builtin\n");
		// -------------------
		if (data->proc == CHILD_PROC)
		{
			// dprintf(2, "in child process\n");
			data->cmd_path = get_cmd_path(ecmd->argv, data); // free
			data->envp = copy_env_lst_to_arr(data->env_lst);
			// ------ print out arr ------
			// for (int n = 0; data->envp[n] != NULL; n++)
			// 	printf("%s\n", data->envp[n]);
			// ----------------------------
			execve(data->cmd_path, ecmd->argv, data->envp);
			panic(ecmd->argv[0], data, 127);
		}
		else
		{
			pid = fork1(data);
			if (pid == -1)
				return ;
			if (pid == 0)
			{
				data->proc = CHILD_PROC;
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
				panic(ecmd->argv[0], data, 127);
			}
			if (dup2(data->fd_stdin, 0) == -1)
			{
				panic(ERR_DUP2, data, 1);
				return ;
			}
			if (dup2(data->fd_stdout, 1) == -1)
			{
				panic(ERR_DUP2, data, 1);
				return ;
			}
			if (waitpid(pid, &status, 0) == -1)
			{
				panic(ERR_WAITPID, data, 1);
				return ;
			}
			if (WIFEXITED(status))
				data->status = WEXITSTATUS(status);
		}
	}
}

void	run_redir(t_cmd *cmd, t_data *data)
{
	t_redircmd	*rcmd;
	int			process;
	int			fd;

	rcmd = (t_redircmd *)cmd;
	if (make_filename(rcmd, data))
	{
		// error case malloc. let us put status = 1 at this moment.
		// number of files > 1  -> status = 1 (bash at alex's PC)
		ft_dprintf(2, "run_redir: malloc or multiple filename. add error hadling\n");
	}
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
	if (pid1 == -1)
		return ;
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
		return ;
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
