/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 16:31:41 by ixu               #+#    #+#             */
/*   Updated: 2024/01/15 16:31:41 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

// save input from stdin to .heredoc

static void	convert_input(t_data *data, int fd_heredoc, char **argv)
{
	char	*line;
	char	*limiter;

	line = get_next_line(STDIN_FILENO);
	limiter = ft_strjoin(argv[2], "\n");
	while (ft_strcmp(line, limiter) != 0)
	{
		if (ft_putstr_fd(line, fd_heredoc) == -1)
		{
			free(line);
			free(limiter);
			if (close(fd_heredoc) == -1)
				print_error(ERR_HEREDOC, data, EXIT_FAILURE);
			print_error(ERR_WRITE, data, EXIT_FAILURE);
		}
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	free(limiter);
	free(line);
}

/* 
	in case of here_doc (1) get input from stdin 
	(2) save to .heredoc (3) open .heredoc 
*/

static void	get_input(t_data *data, char **argv)
{
	int		fd_heredoc;

	fd_heredoc = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd_heredoc == -1)
	{
		close_fds(data, -1, -1);
		print_error(ERR_HEREDOC, data, EXIT_FAILURE);
	}
	convert_input(data, fd_heredoc, argv);
	if (close(fd_heredoc) == -1)
	{
		close_fds(data, -1, -1);
		print_error(ERR_HEREDOC, data, EXIT_FAILURE);
	}
	data->infile = open(".heredoc", O_RDONLY);
	if (data->infile == -1)
	{
		close_fds(data, -1, -1);
		print_error(ERR_HEREDOC, data, EXIT_FAILURE);
	}
}

void	open_infile(t_data *data, char **argv)
{
	if (!data->here_doc)
	{
		data->infile = open(argv[1], O_RDONLY);
		if (data->infile < 0)
		{
			close_fds(data, -1, -1);
			print_error(argv[1], data, EXIT_FAILURE);
		}
	}
	else
		get_input(data, argv);
}

void	open_outfile(t_data *data, char **argv)
{
	int	i;

	if (!data->here_doc)
	{
		i = data->proc_num + 2;
		data->outfile = open(argv[i], O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (data->outfile == -1)
		{
			close_fds(data, -1, -1);
			print_error(argv[i], data, EXIT_FAILURE);
		}
	}
	else
	{
		i = data->proc_num + 3;
		data->outfile = open(argv[i], O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (data->outfile == -1)
		{
			close_fds(data, -1, -1);
			print_error(argv[i], data, EXIT_FAILURE);
		}
	}
}

// close all pipe fds except for fd1 and fd2 (if valid)

void	close_fds(t_data *data, int fd1, int fd2)
{
	int	i;

	i = 0;
	while (i < data->pipe_num * 2)
	{
		if (data->fds[i] != fd1 && data->fds[i] != fd2)
		{
			if (close(data->fds[i]) == -1)
				print_error(ERR_CLOSE, data, EXIT_FAILURE);
		}
		i++;
	}
}
