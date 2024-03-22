#include "../minishell.h"

volatile sig_atomic_t	last_sig;

/* buf is considered valid if (1) it is not empty string,
and (2) it contains at least one character other than white spaces */

int	is_valid_buf(char *buf)
{
	int	i;
	int	j;
	int	is_white_space;

	if (ft_strlen(buf) == 0)
		return (0);
	i = -1;
	while (buf[++i] != '\0')
	{
		j = -1;
		is_white_space = 0;
		while (WHITESPACE[++j] != '\0')
		{
			if (buf[i] == WHITESPACE[j])
			{
				is_white_space = 1;
				break ;
			}
		}
		if (!is_white_space)
			return (1);
	}
	return (0);	
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	t_cmd	*cmd;
	int		status;

	// ------ print out envp ------
	// for (int k = 0; envp[k] != NULL; k++)
	// 	printf("%s\n", envp[k]);
	// ----------------------------
	// (void)argv;
	validate_args(argc);
	data_init(&data, envp);
	while (data.status >= 0)
	{
		if (set_signals_interactive(&data) == 1)
			break ;
		data.buf = readline("LiteShell$ ");
		if (last_sig)
			data.status = 1;
		if (set_default_signals(&data) == 1)
			break ;
		if (data.buf == NULL) // Check for EOF (Ctrl+D)
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
				ft_dprintf(2,"------------->TESTMODE<----------\n");
			//	cmd = parsecmd(data.buf, NULL);
				if (cmd)
					runcmd_test(cmd, &data);
				ft_dprintf(2,"------------->  END   <----------\n");
			}
			if (status == 0)
			{
		        runcmd(cmd, &data);
				// getchar();
			}
			else if (status == ENOMEM_ERR)
			{
				freecmd(cmd);
				//        clean data
				break ; //do we need to break?
			}
			else
				data.status = status;
			freecmd(cmd);
			// dprintf(2, "data.buf(after): %s\n", data.buf);
			// ------ print out envp ------
			// for (int k = 0; envp[k] != NULL; k++)
			// 	printf("%s\n", envp[k]);
			// ----------------------------
			// dprintf(2, "\033[0;35m[status: %d]\033[0m\n", data.status);
			// ------
		}
		//free(data.buf);
		if (data.status == 130)
			dprintf(2, "\n");
		if (data.status == 131)
			dprintf(2, "Quit: 3\n");
		last_sig = 0;
		data.under_pipe = 0;
		data.under_redir = 0;
	}
	close(data.fd_stdin);
	close(data.fd_stdout);
	unlink(".heredoc");
	free_data(&data);
	rl_clear_history();
	// printf("about to exit\n");
	//while (1)
	//	;
	exit(EXIT_SUCCESS);
}
