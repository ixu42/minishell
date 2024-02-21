#include "minishell.h"

int	fork1(t_data *data)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		panic(ERR_FORK, data, EXIT_FAILURE);
	return (pid);
}

void	free_data(t_data *data)
{
	int	i;

	i = -1;
	while (data->envp[++i] != NULL)
		free(data->envp[i]);
	free(data->buf);
}

void	panic(char *err_msg, t_data *data, int exit_code)
{
	if (exit_code == EXIT_CMD_NOT_FOUND)
		ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0m%s: command not found\n", err_msg); // protect
	else
		ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0m%s\n", err_msg); // protect
	free_data(data);
	// free tree nodes?
	exit(exit_code);
}

int	exec_echo(char **argv) // the expanded argv
{
	int	add_new_line;
	int	start_index;
	int	i;

	add_new_line = 0;
	start_index = 1;
	if (argv[1] != NULL && ft_strcmp(argv[1], "-n") == 0)
		start_index++;
	else
		add_new_line = 1;
	i = start_index;
	while (argv[i] != NULL)
	{
		if (printf("%s", argv[i]) < 0)
		{
			ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0, %s", ERR_PRINTF);
			return (1);
		}
		i++;
		if (argv[i] != NULL)
		{
			if (printf(" ") < 0)
			{
				ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0, %s", ERR_PRINTF);
				return (1);
			}
		}
	}
	if (add_new_line)
	{
		if (printf("\n") < 0)
		{
			ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0, %s", ERR_PRINTF);
			return (1);
		}
	}
	return (0);
}

/* Exit the shell, returning a status of n to the shell’s parent. 
If n is omitted, the exit status is that of the last command executed */

int	exec_exit(char **argv) // the expanded argv
{
	if (argv[1] == NULL) // n is omitted
		exit(0);
	else if (argv[2] != NULL) // too many args
	{
		ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0mexit: too many arguments\n"); // protect
		return (1);
	}
	else
		exit(ft_atoi(argv[1]));
	return (0);
}

int	exec_cd(char **argv)
{
	if (argv[1] == NULL) // If directory is not supplied, the value of the HOME shell variable is used.
	{
		ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0mcd: too few arguments\n"); // protect
		return (1);
	}
	else if (argv[2] != NULL) // too many args
		ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0mcd: too many arguments\n"); // protect
	else
	{
		if (chdir(argv[1]) == -1)
		{
			ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0mcd: ");
			perror(argv[1]);
			return (1);
		}
	}
	return (0);
}

int	exec_pwd(char **argv)
{
	char	cwd[1024]; // malloc?

	if (getcwd(cwd, sizeof(cwd)) == NULL) // error msg?
		return (1);
	if (printf("%s\n", cwd) < 0)
	{
		ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0, %s", ERR_PRINTF);
		return (1);
	}
	return (0);
}

int	is_builtin(char **argv, t_data **data) // the expanded argv
{
	if (ft_strcmp(argv[0], "echo") == 0)
		(*data)->builtin = ECHO;
	else if (ft_strcmp(argv[0], "cd") == 0)
		(*data)->builtin = CD;
	else if (ft_strcmp(argv[0], "pwd") == 0)
		(*data)->builtin = PWD;
	else if (ft_strcmp(argv[0], "export") == 0)
		(*data)->builtin = EXPORT;
	else if (ft_strcmp(argv[0], "unset") == 0)
		(*data)->builtin = UNSET;
	else if (ft_strcmp(argv[0], "env") == 0)
		(*data)->builtin = ENV;
	else if (ft_strcmp(argv[0], "exit") == 0)
		(*data)->builtin = EXIT;
	else
		return (0);
	return (1);
}

int	run_builtin(char **argv, t_data *data) // the expanded argv
{
	if (data->builtin == ECHO)
		return (exec_echo(argv));
	else if (data->builtin == CD)
		return (exec_cd(argv));
	else if (data->builtin == PWD)
		return (exec_pwd(argv));
	else if (data->builtin == EXIT)
		return (exec_exit(argv));
}

/* the parameter process to indicate if runcmd 
is executed in child process or in parent process
in child process -> exit with status code; 
in parent process -> return status code */

int	runcmd(t_cmd *cmd, t_data *data, int process)
{
	int				pipe_fd[2];
	t_execcmd		*ecmd;
	t_listcmd		*lcmd;
	t_pipecmd		*pcmd;
	t_redircmd		*rcmd;
	int				pid;
	int				pid1;
	int				pid2;
	int				status;
	int				pipe_cmd;

	if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		// ------ print out args ------
		// for (int i = 0; ecmd->argv[i] != NULL; i++)
		// 	dprintf(2, "ecmd->argv[%d]: %s\n", i, ecmd->argv[i]);
		// ----------------------------
		if (is_builtin(ecmd->argv, &data))
		{
			// ------ debug ------
			// dprintf(2, "builtin\n");
			// -------------------
			status = run_builtin(ecmd->argv, data);
			if (process == CHILD_PROC)
				exit(status); // free all heap allocated memory
			else
				return (status);
		}
		else
		{
			// ------ debug ------
			// dprintf(2, "non-builtin\n");
			// -------------------
			if (process == CHILD_PROC)
			{
				execve(ecmd->argv[0], ecmd->argv, data->envp);
				panic(ecmd->argv[0], data, EXIT_CMD_NOT_FOUND);
			}
			else
			{
				pid = fork1(data);
				if (pid == 0)
				{
					execve(ecmd->argv[0], ecmd->argv, data->envp);
					panic(ecmd->argv[0], data, EXIT_CMD_NOT_FOUND);
				}
				if (waitpid(pid, &status, 0) == -1)
					panic(ERR_WAITPID, data, EXIT_FAILURE);
				if (WIFEXITED(status))
					return (WEXITSTATUS(status));
			}
		}
	}
	else if (cmd->type == REDIR) // redirection does not work atm
	{
		// ------ debug ------
		// printf("redir\n");
		// -------------------
		rcmd = (t_redircmd *)cmd;
		if (close(rcmd->fd) == -1)
			perror("close"); // protect
		if (open(rcmd->file, rcmd->mode) == -1)
			perror("open");
		runcmd(rcmd->cmd, data, process);
	}
	else if (cmd->type == AND_CMD)
	{
		// ------ debug ------
		// printf("&& operator\n");
		// -------------------
		lcmd = (t_listcmd *)cmd;
		status = runcmd(lcmd->left, data, PARENT_PROC);
		if (status == 0)
			status = runcmd(lcmd->right, data, PARENT_PROC);
	}
	else if (cmd->type == OR_CMD)
	{
		// ------ debug ------
		// printf("|| operator\n");
		// -------------------
		lcmd = (t_listcmd *)cmd;
		status = runcmd(lcmd->left, data, PARENT_PROC);
		if (status != 0)
			status = runcmd(lcmd->right, data, PARENT_PROC);
	}
	else if (cmd->type == PIPE)
	{
		// ------ debug ------
		// printf("pipe\n");
		// -------------------
		pcmd = (t_pipecmd *)cmd;
		if (pipe(pipe_fd) < 0)
			panic(ERR_PIPE, data, EXIT_FAILURE);
		pid1 = fork1(data);
		if (pid1 == 0)
		{
			close(pipe_fd[0]);
			dup2(pipe_fd[1], STDOUT_FILENO);
			close(pipe_fd[1]);
			runcmd(pcmd->left, data, CHILD_PROC);
		}
		pid2 = fork1(data);
		if (pid2 == 0)
		{
			close(pipe_fd[1]);
			dup2(pipe_fd[0], STDIN_FILENO);
			close(pipe_fd[0]);
			runcmd(pcmd->right, data, CHILD_PROC);
		}
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		if (waitpid(pid1, NULL, 0) == -1)
			panic(ERR_WAITPID, data, EXIT_FAILURE);
		if (waitpid(pid2, &status, 0) == -1)
			panic(ERR_WAITPID, data, EXIT_FAILURE);
		if (WIFEXITED(status) && process == PARENT_PROC)
			status = WEXITSTATUS(status);
		if (WIFEXITED(status) && process == CHILD_PROC)
			exit(WEXITSTATUS(status)); // free all heap allocated memory
	}
	return (status);
}

// previous version of runcmd() is commented out below

/* void	runcmd(t_cmd *cmd, t_data *data)
{
	int				pipe_fd[2];
	t_execcmd		*ecmd;
	t_listcmd		*lcmd;
	t_pipecmd		*pcmd;
	t_redircmd		*rcmd;
	int				pid1;
	int				pid2;
	int				status;

	if (cmd == NULL)
		exit(1); // exit code? free heap allocated memory? why would this occur?
	else if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		if (ecmd->argv[0] == NULL)
			exit(1); // exit code? free heap allocated memory? why would this occur?
		// expansion
		// ------ printout args ------
		// for (int i = 0; ecmd->argv[i] != NULL; i++)
		// 	printf("ecmd->argv[%d]: %s\n", i, ecmd->argv[i]);
		// ---------------------------
		if (is_builtin(ecmd->argv, &data))
		{
			run_builtin(ecmd->argv, data);
			exit(0); // replace 0 with correct exit code
		}	
		else
		{
			// execve(ecmd->argv[0], ecmd->argv, data->envp);
			// panic(ecmd->argv[0], data, EXIT_CMD_NOT_FOUND);
			pid1 = fork1(data);
			if (pid1 == 0)
			{
				execve(ecmd->argv[0], ecmd->argv, data->envp);
				panic(ecmd->argv[0], data, EXIT_CMD_NOT_FOUND);
			}
			if (waitpid(pid1, &status, 0) == -1)
				ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033\n", ERR_WAITPID);
			if (WIFEXITED(status))
				exit(WEXITSTATUS(status));
		}
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd *)cmd;
		close(rcmd->fd);
	//	fd = open(rcmd->file, rcmd->mode)
	//	if (fd < 0)
		if (open(rcmd->file, rcmd->mode, data) < 0)
			panic(rcmd->file, data, EXIT_FAILURE); // replace by perror?
//		dub2(fd, rcmd->fd); no need for that as fd will be rcmd->fd as it is closed. 
		runcmd(rcmd->cmd, data);
	}
	else if (cmd->type == AND_CMD)
	{
		lcmd = (t_listcmd *)cmd;
		//remove fork use data instead  to pass error status
		//make int type of runcmd to return status;
		if (lcmd->right->type == EXEC)
		{
			if (((t_execcmd *)(lcmd->right))->argv[0] == NULL)
				panic("right command is not found after &&", data, 1);
		}
		pid1 = fork1(data);
		if (pid1 == 0)
			runcmd(lcmd->left, data);
		if (waitpid(pid1, &status, 0) == -1)
			panic(ERR_WAITPID, data, EXIT_FAILURE);
		if (status == 0)
			runcmd(lcmd->right, data);
		else
		{
			// remove dprintf
			dprintf(2, "&&: status error of left cmd=%d\n", status);
			// perror()
		}
	}
	else if (cmd->type == OR_CMD)
	{
		lcmd = (t_listcmd *)cmd;
		//remove fork use data instead  to pass error status
		//make int type of runcmd to return status;
		if (lcmd->right->type == EXEC)
		{
			if (((t_execcmd *)(lcmd->right))->argv[0] == NULL)
				panic("right command is not found ||", data, 1);
		}
		pid1 = fork1(data);
		if (pid1 == 0)
			runcmd(lcmd->left, data);
		if (waitpid(pid1, &status, 0) == -1)
			panic(ERR_WAITPID, data, EXIT_FAILURE);
		if (status != 0)
		{
			dprintf(2, "||: status error of left cmd=%d\n", status);
			runcmd(lcmd->right, data);
			//remove dprintf
//			perror() ?
		}
	}
	// remove LIST ; node 
	else if (cmd->type == LIST) // &&, ||, exit code to be considered
	{
		dprintf(2, "syntax error?! remove ;-nodes\n");
		lcmd = (t_listcmd *)cmd;
		if (fork1(data) == 0)
			runcmd(lcmd->left, data);
		wait(NULL);
		runcmd(lcmd->right, data);
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		if (pipe(pipe_fd) < 0)
			panic(ERR_PIPE, data, EXIT_FAILURE);
		pid1 = fork1(data);
		if (pid1 == 0)
		{
			close(1);
			dup(pipe_fd[1]);
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			runcmd(pcmd->left, data);
		}
		pid2 = fork1(data);
		if (pid2 == 0)
		{
			close(0);
			dup(pipe_fd[0]);
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			runcmd(pcmd->right, data);
		}
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		if (waitpid(pid1, NULL, 0) == -1)
			panic(ERR_WAITPID, data, EXIT_FAILURE);
		if (waitpid(pid2, &status, 0) == -1)
			panic(ERR_WAITPID, data, EXIT_FAILURE);
	}
	else
		panic("runcmd", data, EXIT_FAILURE);
	// if (WIFEXITED(status)) // returns true if the child process is terminated normally
	// 	exit(WEXITSTATUS(status)); // exit with exit status of the child
	// if (WIFSIGNALED(status)) // returns true if the child process is terminated by a signal
	// 	exit(WTERMSIG(status)); // returns the number of signal that caused the child process to terminate
} */
