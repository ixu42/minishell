/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 10:33:17 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/24 17:56:05 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	panic_heredoc(char *err_msg, t_strstate *state, int err_code)
{
	state->flag |= err_code;
	if (!err_msg)
		return (1);
	if (ft_dprintf(2, "%s", PMT) == -1)
		perror(PMT_ERR_WRITE);
	perror(err_msg);
	return (1);
}
int	write_heredoc(char *line, int fd_heredoc, t_strstate *state)
{
	if (ft_putstr_fd(line, fd_heredoc) == -1)
	{
		free(line);
		if (close(fd_heredoc) == -1)
			panic_heredoc(ERR_HEREDOC, state, HEREDOC_OPEN_ERR);
		return (1);
	}
	return (0);
}

// save input from stdin to .heredoc
static int	convert_input_here(t_strstate *state, \
	int fd_heredoc, char *delimiter)
{
	char	*line;
	int		len;
	int		malloc_err;

	len = ft_strlen(delimiter);
	line = NULL;
	malloc_err = 0;
	if (heredoc_signal_handler() == 1)
		return (panic_heredoc(ERR_SIGACTION, state, HEREDOC_OPEN_ERR));
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0, &malloc_err);
		if (g_last_sig && panic_heredoc(NULL, state, SIGNAL_CTRL_C))
			break ;
		if (malloc_err)
			return (panic_heredoc(NULL, state, MALLOC_ERROR));
		if (line == NULL)
			return (0);
		if (ft_strncmp(line, delimiter, len) == 0 \
			&& (line[len] == '\n' || line[len] == '\0'))
			break ;
		if (write_heredoc(line, fd_heredoc, state))
			return (panic_heredoc(ERR_WRITE, state, HEREDOC_OPEN_ERR));   //do we need continue to the next iteration?
		free(line);
		line = NULL;
	}
	if (line)
		free(line);
	return (0);
}
/*
		if (ft_putstr_fd(line, fd_heredoc) == -1)
		{
			free(line);
			if (close(fd_heredoc) == -1)
				panic_heredoc(ERR_HEREDOC, state, HEREDOC_OPEN_ERR);
			return (panic_heredoc(ERR_WRITE, state, HEREDOC_OPEN_ERR));   //do we need continue to the next while loop ?
		}
*/

/* 
in case of here documents (1) create a temporary file .heredoc, 
(2) get input from stdin and save to .heredoc, and (3) close .heredoc 
*/

char	*heredoc_filename(int n)
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
	convert_input_here(state, fd_heredoc, delimiter);
	if (close(fd_heredoc) == -1)
		return (panic_heredoc(ERR_HEREDOC, state, HEREDOC_OPEN_ERR));
	return (0);
}
