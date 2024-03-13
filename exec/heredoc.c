#include "../minishell.h"

// save input from stdin to .heredoc

static void	convert_input(t_data *data, int fd_heredoc, char *delimiter)
{
	char	*line;
	char	*delim;

	line = get_next_line(0);
	delim = ft_strjoin(delimiter, "\n");
	while (ft_strcmp(line, delim) != 0)
	{
		if (ft_putstr_fd(line, fd_heredoc) == -1)
		{
			free(line);
			free(delim);
			if (close(fd_heredoc) == -1)
				panic(ERR_HEREDOC, data, EXIT_FAILURE);
			panic(ERR_WRITE, data, EXIT_FAILURE);
		}
		free(line);
		line = get_next_line(0);
	}
	free(delim);
	free(line);
}

/* in case of here documents (1) create a temporary file .heredoc, (2) get input from stdin and save to .heredoc, and (3) close .heredoc */

void	get_input(t_data *data, char *delimiter)
{
	int		fd_heredoc;

	unlink(".heredoc");
	fd_heredoc = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd_heredoc == -1)
	{
		// close_fds(data, -1, -1);
		panic("error opening .heredoc", data, 1);
		return ;
	}
	convert_input(data, fd_heredoc, delimiter);
	if (close(fd_heredoc) == -1)
	{
		// close_fds(data, -1, -1);
		panic("error closing .heredoc", data, 1);
		return ;
	}
	// data->infile = open(".heredoc", O_RDONLY);
	// if (data->infile == -1)
	// {
	// 	close_fds(data, -1, -1);
	// 	print_error(ERR_HEREDOC, data, EXIT_FAILURE);
	// }
}
