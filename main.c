#include "minishell.h"

static void	validate_args(int argc)
{
	if (argc != 1)
	{
		if (ft_putendl_fd(ERR_ARGS, 2) == -1)
			print_error_n_exit(ERR_WRITE);
		if (ft_putendl_fd(USAGE, 1) == -1)
			print_error_n_exit(ERR_WRITE);
		exit(EXIT_FAILURE);
	}
}

static void	data_init(t_data *data, char **envp)
{
	// data->envp = copy_env(envp); // free
	data->envp = NULL;
	data->env_lst = copy_env_arr_to_lst(envp);
	// ------ print out list ------
	// t_env	*tmp = data->env_lst;
	// while (tmp != NULL)
	// {
	// 	printf("%s=%s\n", tmp->name, tmp->value);
	// 	tmp = tmp->next;
	// }
	// ----------------------------
	data->env_paths = get_env_paths(envp, data); // free before exit in parent and child processes
	data->builtin = 0;
}

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
			// dprintf(2, "data.buf(after): %s\n", data.buf);
			status = runcmd(cmd, &data, PARENT_PROC);
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
