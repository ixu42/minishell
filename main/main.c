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

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	t_cmd	*cmd;
	int 	status;

	(void)argv;
	validate_args(argc);
	data_init(&data, envp);
	while (data.status_code >= 0) 
	{
		data.buf = readline("\033[0;32mLiteShell$ \033[0m"); // free
		if (data.buf == NULL)
		{
			printf("\n"); // check bash on MacOS
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
			if (TESTMODE)
			{
				ft_dprintf(2,"------------->TESTMODE<----------\n");
				runcmd_test(cmd);
				ft_dprintf(2,"------------->  END   <----------\n");
			}
			// dprintf(2, "data.buf(after): %s\n", data.buf);
			status = runcmd(cmd, &data, PARENT_PROC);
			// ------ print out envp ------
			// for (int k = 0; envp[k] != NULL; k++)
			// 	printf("%s\n", envp[k]);
			// ----------------------------
			printf("\033[0;35m[status: %d]\033[0m\n", status);
			// ------
		}
		free(data.buf);
	}
	free_data(&data);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}
