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
		ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: %s: command not found\n\033[0m", err_msg);
	else
		ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: %s\n\033[0m", err_msg);
	free_data(data);
	// free tree nodes?
	exit(exit_code);
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
		execve(ecmd->argv[0], ecmd->argv, data->envp);
		panic(ecmd->argv[0], data, EXIT_CMD_NOT_FOUND);
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
