#include "../minishell.h"

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

void	signal_handler(int signum)
{
	// printf("Received signal %d\n", signum);
	if (signum == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

// sig_t	signal(int sig, sig_t func)

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	t_cmd	*cmd;
	// int		status;

	// ------ print out envp ------
	// for (int k = 0; envp[k] != NULL; k++)
	// 	printf("%s\n", envp[k]);
	// ----------------------------
	(void)argv;
	validate_args(argc);
	data_init(&data, envp);
	while (data.status >= 0)
	{
		struct sigaction	sa;
		sa.sa_handler = &signal_handler;
		// sa.sa_flags = SA_RESTART;
		sigaction(SIGINT, &sa, NULL);
		// signal(SIGINT, signal_handler);
		// data.buf = readline("\001\033[0;32mLiteShell$ \033[0m\002"); // free
		data.buf = readline("LiteShell$ "); // free. discuss if all color formatting should be removed
		if (data.buf == NULL) // Check for EOF (Ctrl+D)
		{
			printf("\033[A\033[11Cexit\n");
			break ;
		}
		if (is_valid_buf(data.buf))
		{
			add_history(data.buf);
			// handling buf (parsing + execution)
			// test parsing/execution funcs here!
			// ------
			// dprintf(2, "data.buf(before): %s\n", data.buf);
			cmd = parsecmd(data.buf); // data.buf is modified
			// if (TESTMODE)
			// {
			// 	ft_dprintf(2,"------------->TESTMODE<----------\n");
			// 	runcmd_test(cmd);
			// 	ft_dprintf(2,"------------->  END   <----------\n");
			// }
			// dprintf(2, "data.buf(after): %s\n", data.buf);
			runcmd(cmd, &data, PARENT_PROC);
			// ------ print out envp ------
			// for (int k = 0; envp[k] != NULL; k++)
			// 	printf("%s\n", envp[k]);
			// ----------------------------
			printf("\033[0;35m[status: %d]\033[0m\n", data.status);
			// ------
		}
		free(data.buf);
	}
	free_data(&data);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}
