/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 20:29:35 by ixu               #+#    #+#             */
/*   Updated: 2024/03/23 13:49:01 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
