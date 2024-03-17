#include "../minishell.h"

// save input from stdin to .heredoc

/* static void	convert_input(t_data *data, int fd_heredoc, char *delimiter)
{
	char	*line;
	char	*delim;

	line = get_next_line(0);
	if (heredoc_signal_handler() == 1)
		panic(ERR_SIGACTION, data, 1);
	if (last_sig)
	{
		data->status = 1;
		return ;
	}
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
		if (last_sig)
		{
			data->status = 1;
			return ;
		}
	}
	free(delim);
	free(line);
} */

static void	convert_input(t_data *data, int fd_heredoc, char *delimiter)
{
	char	*line;
	char	*delim;

	if (heredoc_signal_handler() == 1)
		panic(ERR_SIGACTION, data, 1);
	while (1)
	{
		line = readline("> ");
		if (last_sig)
		{
			data->status = 1;
			return ;
		}
		if (line == NULL) // // Check for EOF (Ctrl+D)
			return ;
		if (ft_strcmp(line, delimiter) == 0)
			break ;
		if (ft_putstr_fd(line, fd_heredoc) == -1)
		{
			free(line);
			if (close(fd_heredoc) == -1)
				panic(ERR_HEREDOC, data, EXIT_FAILURE);
			panic(ERR_WRITE, data, EXIT_FAILURE);
		}
		free(line);
	}
	free(line);
}

/* in case of here documents (1) create a temporary file .heredoc, (2) get input from stdin and save to .heredoc, and (3) close .heredoc */

void	get_input(t_data *data, char *delimiter)
{
	int		fd_heredoc;

	if (heredoc_signal_handler() == 1)
		panic(ERR_SIGACTION, data, 1);
	unlink(".heredoc");
	fd_heredoc = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd_heredoc == -1)
	{
		panic("error opening .heredoc", data, 1);
		return ;
	}
	convert_input(data, fd_heredoc, delimiter);
	if (close(fd_heredoc) == -1)
	{
		panic("error closing .heredoc", data, 1);
		return ;
	}
}
