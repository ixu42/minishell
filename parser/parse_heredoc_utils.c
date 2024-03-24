/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 21:51:16 by ixu               #+#    #+#             */
/*   Updated: 2024/03/24 21:52:42 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	panic_heredoc(char *err_msg, t_strstate *state, int err_code)
{
	state->flag |= err_code;
	unlink(state->heredoc);
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
			return (panic_heredoc(ERR_HEREDOC, state, HEREDOC_OPEN_ERR));
		return (panic_heredoc(ERR_WRITE, state, HEREDOC_OPEN_ERR));
	}
	return (0);
}

/* the func returns 1 in case of malloc error, read error, receiving 
SIGINT, or receiving EOF signal, otherwise the func returns 0 */

int	heredoc_stop_iter(t_strstate *state, char *line, \
	int malloc_err, int read_err)
{
	if (g_last_sig && panic_heredoc(NULL, state, SIGNAL_CTRL_C))
	{
		free(line);
		return (1);
	}
	if (malloc_err)
		return (panic_heredoc(ERR_MALLOC, state, MALLOC_ERROR));
	if (read_err)
		return (panic_heredoc(ERR_READ, state, READ_ERR));
	if (line == NULL)
		return (1);
	return (0);
}
