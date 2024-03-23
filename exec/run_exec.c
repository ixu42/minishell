#include "../minishell.h"

static void	update_shlvl(int argc, char **argv, t_env *env_lst) // lvl down before exit???????
{
	t_env	*tmp;
	int		new_shlvl;

	// dprintf(2, "debug\n");
	// for (int i = 0; argv[i] != NULL; i++)
	// 	dprintf(2, "argv[%d]:%s\n", i, argv[i]);
	if (argc == 1 && argv != NULL && ft_strcmp(argv[0], "./minishell") == 0)
	{
		// dprintf(2, "debug1\n");
		tmp = env_lst;
		while (tmp != NULL)
		{
			// dprintf(2, "debug2\n");
			if (ft_strcmp(tmp->name, "SHLVL") == 0 && tmp->value != NULL)
			{
				new_shlvl = ft_atoi(tmp->value) + 1;
				free(tmp->value);
				tmp->value = ft_itoa(new_shlvl);
			}
			// dprintf(2, "debug3\n");
			tmp = tmp->next;
		}
	}
	// dprintf(2, "debug4\n");
}

int	run_exec(t_cmd *cmd, t_data *data)
{
	t_execcmd	*ecmd;
	pid_t		pid;
	int			status;

	ecmd = (t_execcmd *)cmd;
	if (ecmd->argc)
		make_argv(ecmd, data);
	if (ecmd->argc == 0)
	{
		if (dup2(data->fd_stdin, 0) == -1)
			return (panic(ERR_DUP2, data, 1));
		if (dup2(data->fd_stdout, 1) == -1)
			return (panic(ERR_DUP2, data, 1));
		if (data->proc == CHILD_PROC)
			free_n_exit(data, 0);
	 	return (0);
	}
	/*
	 if ( ecmd->argv[1] == NULL)
		printf("NULL pointer\n");
	if ( ecmd->argv[1][0] == '\0')
		printf("empty str\n");
	*/
	// if (ecmd->argv == NULL || ecmd->argv[0] == NULL)
	// {
	// 	dprintf(2, "debug\n");
	// 	if (data->proc == CHILD_PROC)
	// 		free_n_exit(data, 0);
	// 	else
	// 	{
	// 		data->status = 0;
	// 		return (0);
	// 	}
	// }
	// ------ print out args ------
	//for (int i = 0; ecmd->argv[i] != NULL; i++)
	// 	dprintf(2, "ecmd->argv[%d]: %s\n", i, ecmd->argv[i]);
	// ----------------------------
	if (ecmd->argv != NULL && ecmd->argv[0] != NULL && is_builtin(ecmd->argv, &data))
	{
		// ------ debug ------
		// dprintf(2, "builtin\n");
		// -------------------
		data->status = run_builtin(ecmd->argv, data);
		if (!data->under_pipe && data->under_redir)//(data->proc != CHILD_PROC)
		{
			if (dup2(data->fd_stdin, 0) == -1)
				return (panic(ERR_DUP2, data, 1));
			if (dup2(data->fd_stdout, 1) == -1)
				return (panic(ERR_DUP2, data, 1));
		}
		if (data->proc == CHILD_PROC)
			free_n_exit(data, data->status);
	}
	else
	{
		// ------ debug ------
		// dprintf(2, "non-builtin\n");
		// -------------------
		update_shlvl(ecmd->argc, ecmd->argv,data->env_lst);
		if (data->proc == CHILD_PROC)
		{
			data->cmd_path = get_cmd_path(ecmd->argv, data); // free
			data->envp = copy_env_lst_to_arr(data->env_lst);
			// ------ print out arr ------
			// for (int n = 0; data->envp[n] != NULL; n++)
			// 	printf("%s\n", data->envp[n]);
			// ----------------------------
			execve(data->cmd_path, ecmd->argv, data->envp);
			// if (ecmd->argv[0] == NULL)
			// 	panic_cmd_not_found("", data);
			panic_cmd_not_found(ecmd->argv[0], data);
		}
		else
		{
			pid = fork1(data);
			if (pid == -1)
				return (1);
			if (pid == 0)
			{
				data->proc = CHILD_PROC;
				// dprintf(2, "ecmd->argv[0]:%s\n", ecmd->argv[0]);
				data->cmd_path = get_cmd_path(ecmd->argv, data); // free
				// dprintf(2, "data->cmd_path: %s\n", data->cmd_path);
				// ------ print out list ------
				// t_env	*tmp = data->env_lst;
				// while (tmp != NULL)
				// {
				// 	printf("%s=%s\n", tmp->name, tmp->value);
				// 	tmp = tmp->next;
				// }
				// ----------------------------
				data->envp = copy_env_lst_to_arr(data->env_lst);
				// ------ print out arr ------
				// for (int n = 0; data->envp[n] != NULL; n++)
				// 	dprintf(2 ,"%s\n", data->envp[n]);
				// ---------------------------
				////// the following code is add to avoid running execve with NULL 
				////// added to remove error: Valgrind: Syscall param execve(filename) points to unaddressable byte(s)
				if (!data->cmd_path)
					data->cmd_path = ecmd->argv[0];
				execve(data->cmd_path, ecmd->argv, data->envp);
				// if (ecmd->argv[0] == NULL)
				// 	panic_cmd_not_found("", data);
				panic_cmd_not_found(ecmd->argv[0], data);
			}
			if (ignore_signals() == 1)
				return (panic(ERR_SIGACTION, data, 1));
			if (!data->under_pipe && data->under_redir)
			{
				if (dup2(data->fd_stdin, 0) == -1)
					return (panic(ERR_DUP2, data, 1));
				if (dup2(data->fd_stdout, 1) == -1)
					return (panic(ERR_DUP2, data, 1));
			}
			if (waitpid(pid, &status, 0) == -1)
				return (panic(ERR_WAITPID, data, 1));
			if (WIFEXITED(status))
				data->status = WEXITSTATUS(status);
			if (WIFSIGNALED(status))
				data->status = 128 + WTERMSIG(status);
		}
	}
	return (0);
}
