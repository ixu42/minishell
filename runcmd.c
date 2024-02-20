#include "minishell.h"
#include <stdio.h>

void	panic(char *err_msg, t_data *data, int exit_code);

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
		ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: %s: command not found\n\033[0m", err_msg); // protect
	else
		ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: %s\n\033[0m", err_msg); // protect
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
			return (1); // can panic be called (exit curr process)
		i++;
		if (argv[i] != NULL)
		{
			if (printf(" ") < 0)
				return (1);
		}
	}
	if (add_new_line)
	{
		if (printf("\n") < 0)
			return (1);
	}
	return (0);
}

/* Exit the shell, returning a status of n to the shell’s parent. 
If n is omitted, the exit status is that of the last command executed */

int	exec_exit(char **argv) // the expanded argv
{
	if (argv[1] == NULL) // n is omitted
	{
		ft_dprintf(STDOUT_FILENO, "\033[0;34mBye!\n\033[0m"); // protect; any message from bash on MacOS?
		exit(0);
	}
	else if (argv[2] != NULL) // too many args
		ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: exit: too many arguments\n\033[0m"); // protect
	else
	{
		ft_dprintf(STDOUT_FILENO, "\033[0;34mBye!\n\033[0m"); // protect; any message from bash on MacOS?
		exit(ft_atoi(argv[1]));
	}
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
		exec_echo(argv);
	else if (data->builtin == EXIT)
		exec_exit(argv);
}

void	runcmd(t_cmd *cmd, t_data *data)
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
		// for (int i = 0; ecmd->argv[i] != NULL; i++)
		// 	printf("ecmd->argv[%d]: %s\n", i, ecmd->argv[i]);
		if (is_builtin(ecmd->argv, &data))
			run_builtin(ecmd->argv, data);
		else
		{
			pid1 = fork1(data);
			if (pid1 == 0)
				execve(ecmd->argv[0], ecmd->argv, data->envp);
			if (waitpid(pid1, &status, 0) == -1)
				panic(ecmd->argv[0], data, EXIT_CMD_NOT_FOUND);
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
		pid1 = fork1(data);
		if (pid1 == 0)
			runcmd(lcmd->left, data);
		if (waitpid(pid1, &status, 0) == -1)
			panic(ERR_WAITPID, data, EXIT_FAILURE);
		if (status == 0)
			runcmd(lcmd->right, data);
		else
		{
			//remove dprintf
			dprintf(2, "&&: status error of left cmd=%d\n", status);
//			perror()
		}
	}
	else if (cmd->type == OR_CMD)
	{
		lcmd = (t_listcmd *)cmd;
		//remove fork use data instead  to pass error status
		//make int type of runcmd to return status;
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
	if (WIFEXITED(status)) // returns true if the child process is terminated normally
		exit(WEXITSTATUS(status)); // exit with exit status of the child
	// if (WIFSIGNALED(status)) // returns true if the child process is terminated by a signal
	// 	exit(WTERMSIG(status)); // returns the number of signal that caused the child process to terminate
}
