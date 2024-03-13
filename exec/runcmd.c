#include "../minishell.h"

int	fork1(t_data *data)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		panic(ERR_FORK, data, EXIT_FAILURE); // fix this, exit/return depends on process!
	return (pid);
}

/* the parameter process to indicate if runcmd 
is executed in child process or in parent process
in child process -> exit with status code; 
in parent process -> return status code */

void	runcmd(t_cmd *cmd, t_data *data, int process)
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
	int				fd;

	if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		make_argv(ecmd, data);
		if (ecmd->argv == NULL || ecmd->argv[0] == NULL)
		{
			if (process == CHILD_PROC)
				exit(0); // free all heap allocated memory
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
			dup2(data->fd_stdin, 0);
			dup2(data->fd_stdout, 1);
			if (process == CHILD_PROC)
				exit(data->status); // free all heap allocated memory
			else
				return ;
		}
		else
		{
			// ------ debug ------
			// dprintf(2, "non-builtin\n");
			// -------------------
			if (process == CHILD_PROC)
			{
				data->cmd_path = get_cmd_path(ecmd->argv, data); // free
				data->envp = copy_env_lst_to_arr(data->env_lst);
				// ------ print out arr ------
				// for (int n = 0; data->envp[n] != NULL; n++)
				// 	printf("%s\n", data->envp[n]);
				// ----------------------------
				execve(data->cmd_path, ecmd->argv, data->envp);
				panic(ecmd->argv[0], data, EXIT_CMD_NOT_FOUND);
			}
			else
			{
				pid = fork1(data);
				if (pid == 0)
				{
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
					panic(ecmd->argv[0], data, EXIT_CMD_NOT_FOUND);
				}
				dup2(data->fd_stdin, 0);
				dup2(data->fd_stdout, 1);
				if (waitpid(pid, &status, 0) == -1)
					panic(ERR_WAITPID, data, EXIT_FAILURE); // fix this, to return, no exit
				if (WIFEXITED(status))
					data->status = WEXITSTATUS(status);
			}
		}
	}
	else if (cmd->type == REDIR)
	{
		// ------ debug ------
		// dprintf(2, "redir\n");
		// -------------------
		rcmd = (t_redircmd *)cmd;
		fd = open(rcmd->file, rcmd->mode, 0644);
		if (fd == -1)
			panic(ERR_OPEN, data, EXIT_FAILURE);
		if (dup2(fd, rcmd->fd) == -1)
			panic(ERR_DUP2, data, EXIT_FAILURE);
		if (close(fd) == -1)
			panic(ERR_CLOSE, data, EXIT_FAILURE);
		runcmd(rcmd->cmd, data, PARENT_PROC);
		if (process == CHILD_PROC)
			exit(data->status);
	}
	else if (cmd->type == AND_CMD)
	{
		// ------ debug ------
		// dprintf(2, "&& operator\n");
		// -------------------
		lcmd = (t_listcmd *)cmd;
		runcmd(lcmd->left, data, PARENT_PROC);
		if (data->status == 0)
			runcmd(lcmd->right, data, PARENT_PROC);
	}
	else if (cmd->type == OR_CMD)
	{
		// ------ debug ------
		// dprintf(2, "|| operator\n");
		// -------------------
		lcmd = (t_listcmd *)cmd;
		runcmd(lcmd->left, data, PARENT_PROC);
		if (data->status != 0)
			runcmd(lcmd->right, data, PARENT_PROC);
	}
	else if (cmd->type == PIPE)
	{
		// ------ debug ------
		// dprintf(2, "pipe\n");
		// -------------------
		pcmd = (t_pipecmd *)cmd;
		if (pipe(pipe_fd) == -1)
			panic(ERR_PIPE, data, EXIT_FAILURE); // fix this, exit/return depends on process!
		pid1 = fork1(data);
		if (pid1 == 0)
		{
			if (close(pipe_fd[0]) == -1)
				panic(ERR_CLOSE, data, EXIT_FAILURE);
			if (dup2(pipe_fd[1], 1) == -1)
				panic(ERR_DUP2, data, EXIT_FAILURE);
			if (close(pipe_fd[1]) == -1)
				panic(ERR_CLOSE, data, EXIT_FAILURE);
			runcmd(pcmd->left, data, CHILD_PROC);
		}
		pid2 = fork1(data);
		if (pid2 == 0)
		{
			if (close(pipe_fd[1]) ==  -1)
				panic(ERR_CLOSE, data, EXIT_FAILURE);
			if (dup2(pipe_fd[0], 0) == -1)
				panic(ERR_DUP2, data, EXIT_FAILURE);
			if (close(pipe_fd[0]) == -1)
				panic(ERR_CLOSE, data, EXIT_FAILURE);
			runcmd(pcmd->right, data, CHILD_PROC);
		}
		close(pipe_fd[0]); // fix this, exit/return depends on process!
		close(pipe_fd[1]); // fix this, exit/return depends on process!
		if (waitpid(pid1, NULL, 0) == -1)
			panic(ERR_WAITPID, data, EXIT_FAILURE); // fix this, exit/return depends on process!
		if (waitpid(pid2, &status, 0) == -1)
			panic(ERR_WAITPID, data, EXIT_FAILURE); // fix this, exit/return depends on process!
		if (WIFEXITED(status) && process == PARENT_PROC)
			data->status = WEXITSTATUS(status);
		if (WIFEXITED(status) && process == CHILD_PROC)
			exit(WEXITSTATUS(status)); // free all heap allocated memory
	}
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
