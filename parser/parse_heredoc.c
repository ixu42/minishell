/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 10:33:17 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/24 17:56:05 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// save input from stdin to .heredoc

static int	convert_input(t_strstate *state, \
	int fd_heredoc, char *delimiter, int len)
{
	char	*line;
	int		malloc_err;
	int		read_err;

	line = NULL;
	malloc_err = 0;
	read_err = 0;
	if (heredoc_signal_handler() == 1)
		return (panic_heredoc(ERR_SIGACTION, state, HEREDOC_OPEN_ERR));
	while (1)
	{
		if (write(1, "> ", 2) == -1)
			return (panic_heredoc(ERR_WRITE, state, HEREDOC_OPEN_ERR));
		line = get_next_line(0, &malloc_err, &read_err);
		if (heredoc_stop_iter(state, line, malloc_err, read_err))
			return (1);
		if (ft_strncmp(line, delimiter, len) == 0 \
		&& (line[len] == '\n' || line[len] == '\0'))
			break ;
		if (write_heredoc(line, fd_heredoc, state) == 1)
			return (1);
		free(line);
	}
	free(line);
	return (0);
}

static char	*heredoc_filename(int n)
{
	char	*number;
	char	*ret;

	number = ft_itoa(n);
	if (!number)
		return (NULL);
	ret = ft_strjoin(".heredoc", number);
	free(number);
	return (ret);
}

/* in case of here documents (1) create a temporary file .heredoc, 
(2) get input from stdin and save to .heredoc, and (3) close .heredoc */

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
		return (panic_heredoc(ERR_HEREDOC, state, HEREDOC_OPEN_ERR));
	convert_input(state, fd_heredoc, delimiter, ft_strlen(delimiter));
	if (close(fd_heredoc) == -1)
		return (panic_heredoc(ERR_HEREDOC, state, HEREDOC_OPEN_ERR));
	return (0);
}
