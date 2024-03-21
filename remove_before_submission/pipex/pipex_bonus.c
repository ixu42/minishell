/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 13:51:18 by ixu               #+#    #+#             */
/*   Updated: 2024/01/09 16:38:17 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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

/*
	argv[0] [1]      [2]     [3] [4]  [5]
	./pipex here_doc LIMITER cmd cmd1 file
	should behave like:
	cmd << LIMITER | cmd1 >> file
*/

static void	child(t_data *data, int i, char **argv, char **envp)
{
	if (i == 0)
	{
		open_infile(data, argv);
		close_fds(data, data->fds[1], -1);
		redirect_io(data->infile, data->fds[1], data);
	}
	else if (i < data->proc_num - 1)
	{
		close_fds(data, data->fds[(i - 1) * 2], data->fds[i * 2 + 1]);
		redirect_io(data->fds[(i - 1) * 2], data->fds[i * 2 + 1], data);
	}
	else if (i == data->proc_num - 1)
	{
		open_outfile(data, argv);
		close_fds(data, data->fds[(i - 1) * 2], -1);
		redirect_io(data->fds[(i - 1) * 2], data->outfile, data);
	}
	data->cmd_arr = get_cmd_arr(argv[i + 2 + data->here_doc], data);
	data->cmd_path = get_cmd_path(data);
	if (data->cmd_path != NULL)
		execve(data->cmd_path, data->cmd_arr, envp);
	print_error(argv[i + 2 + data->here_doc], data, EXIT_CMD_NOT_FOUND);
}

/* 
	each child process handles one command, e.g.:
	infile->cmd0->pipe0->cmd1->pipe1->cmd2->outfile
*/

static int	pipex(t_data *data, char **argv, char **envp)
{
	int		i;
	int		status;

	create_pipes(data);
	i = -1;
	while (++i < data->proc_num)
	{
		data->pids[i] = fork();
		if (data->pids[i] == -1)
			print_error(ERR_FORK, data, EXIT_FAILURE);
		if (data->pids[i] == 0)
			child(data, i, argv, envp);
	}
	close_fds(data, -1, -1);
	i = -1;
	while (++i < data->proc_num)
	{
		if (waitpid(data->pids[i], &status, 0) == -1)
			print_error(ERR_WAITPID, data, EXIT_FAILURE);
	}
	return (status);
}

/*
	./pipex infile cmd0 cmd1 cmd2 ... cmdn outfile
	should behave like:
	< infile cmd0 | cmd1 | cmd2 ... | cmdn > outfile

	./pipex here_doc LIMITER cmd cmd1 file
	should behave like:
	cmd << LIMITER | cmd1 >> file
*/

int	main(int argc, char **argv, char **envp)
{
	int		status;
	t_data	*data;

	if (argc < 5)
		exit_handler(argv, argc);
	if (ft_strcmp(argv[1], "here_doc") == 0)
	{
		if (argc < 6)
			exit_handler(argv, argc);
	}
	data = (t_data *)malloc(sizeof(t_data));
	if (data == NULL)
		print_error(ERR_MALLOC, data, EXIT_FAILURE);
	data_init(argc, argv, envp, &data);
	status = pipex(data, argv, envp);
	if (WIFEXITED(status))
	{
		if (data->here_doc)
			if (unlink(".heredoc") == -1)
				print_error(ERR_UNLINK, data, EXIT_FAILURE);
		free_struct(data);
		exit(WEXITSTATUS(status));
	}
}
