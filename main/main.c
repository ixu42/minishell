/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 20:29:30 by ixu               #+#    #+#             */
/*   Updated: 2024/03/25 11:40:34 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_last_sig;

static void	cleanup_n_reset(t_data *data)
{
	if (data->status == 130)
		dprintf(2, "\n");
	if (data->status == 131)
		dprintf(2, "Quit: 3\n");
	g_last_sig = 0;
	data->under_pipe = 0;
	data->under_redir = 0;
	free(data->buf);
	freecmd_null(&(data->tree));
}

static void	cleanup_before_exit(t_data *data)
{
	if (close(data->fd_stdin) == -1)
		print_error_n_exit(ERR_CLOSE);
	if (close(data->fd_stdout) == -1)
		print_error_n_exit(ERR_CLOSE);
	free_data(data);
	data = NULL;
	rl_clear_history();
}

// for testmode add the following after make_ast funciton
			/*
			if (TESTMODE)
			{
				ft_dprintf(2, "------------->TESTMODE<----------\n");
			//	cmd = parsecmd(data.buf, NULL);
				if (data.tree)
					runcmd_test(data.tree, &data);
				ft_dprintf(2,"------------->  END   <----------\n");
			}
			*/
int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	//t_cmd	*cmd;
	int		status;

	//cmd = NULL;
	(void)argv;
	validate_args(argc);
	data_init(&data, envp);
	status = 0;
	while (data.status >= 0)
	{
		if (set_signals_interactive() == 1)
		{
			status = 1;
			break ;
		}
		data.buf = readline("LiteShell$ ");
		if (g_last_sig)
			data.status = 1;
		if (set_default_signals() == 1)
		{
			status = 1;
			break ;
		}
		if (data.buf == NULL)
		{
			// struct winsize ws;
    		// ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
			// dprintf(2, "ws.ws_col:%d\n", ws.ws_col);
			// printf("\033[A\033[%dC%s\n", ws.ws_col, "exit");
			printf("exit\n");
			// printf("\033[A\033[%dC%s\n", 11, "exit");
			break ;
		}
		if (is_valid_buf(data.buf))
		{
			add_history(data.buf);
			status = make_ast(&(data.tree), data.buf, &(data.num_heredoc));
			printf("num of heredocs=%d\n", data.num_heredoc);
		/*	if (TESTMODE)
			{
				ft_dprintf(2, "------------->TESTMODE<----------\n");
			//	cmd = parsecmd(data.buf, NULL);
				if (data.tree)
					runcmd_test(data.tree, &data);
				ft_dprintf(2,"------------->  END   <----------\n");
			}
		*/
			if (status == 0)
			    runcmd(data.tree, &data);
			else if (status == ENOMEM_ERR)
			{
				//replace == by &
				status = 1;
				break ;
			}
			else if (status & SIGNAL_CTRL_C)
				data.status = 1;
			else
				data.status = status;
		}
		cleanup_n_reset(&data);
	}
	cleanup_before_exit(&data);
//	while (1)
//		;
	exit(status);
}
