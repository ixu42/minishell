/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 20:29:30 by ixu               #+#    #+#             */
/*   Updated: 2024/03/22 21:44:01 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_last_sig;

static void	cleanup_n_reset(t_data *data)
{
	free(data->buf);
	if (data->status == 130)
		dprintf(2, "\n");
	if (data->status == 131)
		dprintf(2, "Quit: 3\n");
	g_last_sig = 0;
	data->under_pipe = 0;
	data->under_redir = 0;
}

static void	cleanup_before_exit(t_data *data)
{
	if (close(data->fd_stdin) == -1)
		print_error_n_exit(ERR_CLOSE);
	if (close(data->fd_stdout) == -1)
		print_error_n_exit(ERR_CLOSE);
	unlink(".heredoc");
	free_data(data);
	rl_clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	t_cmd	*cmd;
	int		status;

	// (void)argv;
	validate_args(argc);
	data_init(&data, envp);
	while (data.status >= 0)
	{
		if (set_signals_interactive(&data) == 1)
			break ;
		data.buf = readline("LiteShell$ ");
		if (g_last_sig)
			data.status = 1;
		if (set_default_signals(&data) == 1)
			break ;
		if (data.buf == NULL)
		{
			printf("\033[A\033[11Cexit\n");
			break ;
		}
		if (is_valid_buf(data.buf))
		{
			add_history(data.buf);
			status = make_ast(&cmd, data.buf);
			if (TESTMODE)
			{
				ft_dprintf(2, "------------->TESTMODE<----------\n");
			//	cmd = parsecmd(data.buf, NULL);
				if (cmd)
					runcmd_test(cmd, &data);
				// clean cmd
				ft_dprintf(2, "------------->  END   <----------\n");
			}
			if (status == 0)
			{
				runcmd(cmd, &data);
				// getchar();
			}
			else if (status == ENOMEM)
			{
				freecmd(cmd);
				// clean data
				break ; //do we need to break?
			}
			else
				data.status = status;
			freecmd(cmd);
			// dprintf(2, "\033[0;35m[status: %d]\033[0m\n", data.status);
		}
		cleanup_n_reset(&data);
	}
	cleanup_before_exit(&data);
//	while (1)
//		;
	exit(0);
}
