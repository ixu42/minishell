/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 20:29:35 by ixu               #+#    #+#             */
/*   Updated: 2024/05/14 17:49:02 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_termios(int set_echoctl)
{
	struct termios	info;

	ft_bzero(&info, sizeof(struct termios));
	if (set_echoctl == SET_ECHOCTL)
	{
		tcgetattr(0, &info);
		info.c_lflag |= ECHOCTL;
		tcsetattr(0, TCSANOW, &info);
	}
	else
	{
		tcgetattr(0, &info);
		info.c_lflag &= ~ECHOCTL;
		tcsetattr(0, TCSANOW, &info);
	}
}

/* SIGINT signal handler when waiting 
for user inputting command line */

void	display_pmt_on_nl(int signum)
{
	g_last_sig = signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// SIGINT signal handler for here documents

void	move_to_nl(int signum)
{
	g_last_sig = signum;
	write(1, "\n", 1);
}

void	handle_signal_output(int signum)
{
	if (signum == 2)
		write(2, "\n", 1);
	if (signum == 3)
		write(2, "Quit: 3\n", 8);
}
