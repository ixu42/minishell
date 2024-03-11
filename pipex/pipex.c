/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 22:58:14 by ixu               #+#    #+#             */
/*   Updated: 2024/01/08 22:58:14 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	redirect_io(int input, int output, t_data *data)
{
	if (dup2(input, STDIN_FILENO) == -1)
		print_error(ERR_DUP2, data, EXIT_FAILURE);
	if (dup2(output, STDOUT_FILENO) == -1)
		print_error(ERR_DUP2, data, EXIT_FAILURE);
	if (close(input) == -1)
		print_error(ERR_CLOSE, data, EXIT_FAILURE);
	if (close(output) == -1)
		print_error(ERR_CLOSE, data, EXIT_FAILURE);
}

static void	child1(char **argv, int fd[2], t_data *data, char **envp)
{
	int		infile;

	infile = open(argv[1], O_RDONLY);
	if (infile < 0)
	{
		close_fds(data, fd[0], fd[1]);
		print_error(argv[1], data, EXIT_FAILURE);
	}
	if (close(fd[0]) == -1)
	{
		close_fds(data, fd[1], infile);
		print_error(ERR_CLOSE, data, EXIT_FAILURE);
	}
	redirect_io(infile, fd[1], data);
	data->cmd_arr = get_cmd_arr(argv[2], data);
	data->cmd_path = get_cmd_path(data);
	if (data->cmd_path != NULL)
		execve(data->cmd_path, data->cmd_arr, envp);
	print_error(argv[2], data, EXIT_CMD_NOT_FOUND);
}

static void	child2(char **argv, int fd[2], t_data *data, char **envp)
{
	int		outfile;

	outfile = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (outfile < 0)
	{
		close_fds(data, fd[0], fd[1]);
		print_error(argv[4], data, EXIT_FAILURE);
	}
	if (close(fd[1]) == -1)
	{
		close_fds(data, fd[0], outfile);
		print_error(ERR_CLOSE, data, EXIT_FAILURE);
	}
	redirect_io(fd[0], outfile, data);
	data->cmd_arr = get_cmd_arr(argv[3], data);
	data->cmd_path = get_cmd_path(data);
	if (data->cmd_path != NULL)
		execve(data->cmd_path, data->cmd_arr, envp);
	print_error(argv[3], data, EXIT_CMD_NOT_FOUND);
}

static int	pipex(char **argv, t_data *data, char **envp)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	if (pipe(fd) == -1)
		print_error(ERR_PIPE, data, EXIT_FAILURE);
	pid1 = fork();
	if (pid1 == -1)
		print_error(ERR_FORK, data, EXIT_FAILURE);
	if (pid1 == 0)
		child1(argv, fd, data, envp);
	pid2 = fork();
	if (pid2 == -1)
		print_error(ERR_FORK, data, EXIT_FAILURE);
	if (pid2 == 0)
		child2(argv, fd, data, envp);
	close_fds(data, fd[0], fd[1]);
	if (waitpid(pid1, NULL, 0) == -1)
		print_error(ERR_WAITPID, data, EXIT_FAILURE);
	if (waitpid(pid2, &status, 0) == -1)
		print_error(ERR_WAITPID, data, EXIT_FAILURE);
	return (status);
}

/*
	./pipex infile cmd1 cmd2 outfile
	should behave like:
	< infile cmd1 | cmd2 > outfile
*/

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;
	int		status;

	if (argc != 5)
	{
		if (ft_putendl_fd(ERR_INPUT, STDERR_FILENO) == -1)
			print_error(ERR_WRITE, NULL, EXIT_FAILURE);
		if (ft_putendl_fd(EX_ARGS, STDOUT_FILENO) == -1)
			print_error(ERR_WRITE, NULL, EXIT_FAILURE);
		exit(EXIT_FAILURE);
	}
	data = (t_data *)malloc(sizeof(t_data));
	if (data == NULL)
		print_error_no_free(ERR_MALLOC);
	data_init(&data, envp);
	status = pipex(argv, data, envp);
	if (WIFEXITED(status))
	{
		free_arr(data->env_paths);
		free_arr(data->cmd_arr);
		free(data->cmd_path);
		free(data);
		exit(WEXITSTATUS(status));
	}
}
