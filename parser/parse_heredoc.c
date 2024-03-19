#include "../minishell.h"

int	panic_heredoc(char *err_msg, t_strstate *state, int err_code)
{
		if (ft_dprintf(2, "%s", PMT) == -1)
			perror(PMT_ERR_WRITE);
		perror(err_msg);
	state->flag |= err_code;
	return (1);
}

// save input from stdin to .heredoc

static void	convert_input_here(t_strstate *state, int fd_heredoc, char *delimiter)
{
	char	*line;
	int		len;

	len = ft_strlen(delimiter);
	if (heredoc_signal_handler() == 1)
		panic_heredoc(ERR_SIGACTION, state, HEREDOC_OPEN_ERR);
	// do we need to return here after signal handler?
	// we have not the same behavior at alex's pc for `<<eof cat` when Cntl+D
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (last_sig)
		{
			state->flag |= HEREDOC_OPEN_ERR;
			break ;
		}
		if (line == NULL)
			return ;
		if (ft_strncmp(line, delimiter, len) == 0 && (line[len] == '\n' || line[len] == '\0'))
			break ;
		if (ft_putstr_fd(line, fd_heredoc) == -1)
		{
			free(line);
			if (close(fd_heredoc) == -1)
				panic_heredoc(ERR_HEREDOC, state, HEREDOC_OPEN_ERR);
			panic_heredoc(ERR_WRITE, state, HEREDOC_OPEN_ERR);
			//do we need continue to the next while loop ?
		}
		free(line);
	}
	free(line);
}

/* in case of here documents (1) create a temporary file .heredoc, (2) get input from stdin and save to .heredoc, and (3) close .heredoc */

char	*heredoc_filename(int n)
{
	char *number;
	char	*ret;
	
	number = ft_itoa(n);
	if (!number)
		return (NULL);
	ret = ft_strjoin(".heredoc", number);
	free(number);
	return (ret);
}

int	get_input_heredoc(t_strstate *state, t_aststate *ast, char *delimiter)
{
	int		fd_heredoc;
	
	state->heredoc = heredoc_filename(ast->heredoc);
	if (!state->heredoc)
		return (panic_heredoc(ERR_MALLOC, state, MALLOC_ERROR));
	if (heredoc_signal_handler() == 1)
		panic_heredoc(ERR_SIGACTION, state, HEREDOC_OPEN_ERR);
	unlink(state->heredoc);
	fd_heredoc = open(state->heredoc, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd_heredoc == -1)
		return (panic_heredoc("error opening .heredoc", state, HEREDOC_OPEN_ERR));
	convert_input_here(state, fd_heredoc, delimiter);
	if (close(fd_heredoc) == -1)
		return(panic_heredoc("error closing .heredoc", state, HEREDOC_OPEN_ERR));
	return (0);
}
