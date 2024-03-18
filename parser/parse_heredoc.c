#include "../minishell.h"

// save input from stdin to .heredoc

static void	convert_input(t_data *data, int fd_heredoc, char *delimiter)
{
	char	*line;
	int		len;

	len = ft_strlen(delimiter);
	if (heredoc_signal_handler() == 1)
		panic(ERR_SIGACTION, data, 1);
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0); // modified gnl! return only when hitting a nl char, or in case of empty string a null char
		if (last_sig)
		{
			data->status = 1;
			break ;
		}
		if (line == NULL) // Check for EOF (Ctrl+D)
			return ;
		if (ft_strncmp(line, delimiter, len) == 0 && (line[len] == '\n' || line[len] == '\0'))
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
