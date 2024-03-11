#include "../minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	t_cmd	*cmd;
	int 	status;

	(void)argv;
	validate_args(argc);
	data_init(&data, envp);
	data.buf = readline("\033[0;32mLiteShell$ \033[0m"); // free before exit in parent and child processes
	while (data.buf != NULL) 
	{
		if (ft_strlen(data.buf) > 0) // should we check if data.buf only contains white spaces here?
		{
			add_history(data.buf);
			// handling buf (parsing + execution)
			// test parsing/execution funcs here!
			// ------
			// dprintf(2, "data.buf(before): %s\n", data.buf);
			cmd = parsecmd(data.buf); // data.buf is modified
			// status = parsefun(&cmd, data.buf);
			// if (status == 0)
			//		status = runcmd
			// else if (status == malloc error )
			// {
			//        clean cmd
			//       break
			//  }
			// clean cmd including argv
			//  
			if (TESTMODE)
			{
				ft_dprintf(2,"------------->TESTMODE<----------\n");
				if (cmd)
					runcmd_test(cmd, &data);
				//ft_dprintf(2, "NULL AST\n");
				ft_dprintf(2,"------------->  END   <----------\n");
			}
			// dprintf(2, "data.buf(after): %s\n", data.buf);
			if (cmd)
			{
				if (cmd ->flag == 0) 
					status = runcmd(cmd, &data, PARENT_PROC);
			}
			//else terminate everything ??? malloc error

			//status = runcmd(cmd, &data, PARENT_PROC);
			// ------ print out envp ------
			// for (int k = 0; envp[k] != NULL; k++)
			// 	printf("%s\n", envp[k]);
			// ----------------------------
			printf("\033[0;35m[status: %d]\033[0m\n", status);
			// ------
		}
		free(data.buf);
		data.buf = readline("\033[0;32mLiteShell$ \033[0m");
	}
	free_data(&data);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}
