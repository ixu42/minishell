#include "minishell.h"

void	print_error(char *err_msg)
{
	if (ft_putstr_fd("minishell: ", STDERR_FILENO) == -1)
	{
		perror("minishell: write error");
		exit(EXIT_FAILURE);
	}
	perror(err_msg);
	exit(EXIT_FAILURE);
}

void	validate_args(int argc)
{
	if (argc != 1)
	{
		if (ft_putendl_fd(ERR_ARGS, STDERR_FILENO) == -1)
			print_error(ERR_WRITE);
		if (ft_putendl_fd(USAGE, STDOUT_FILENO) == -1)
			print_error(ERR_WRITE);
		exit(EXIT_FAILURE);
	}
}

int	arr_len(char **arr)
{
	int	len;

	len = 0;
	while (arr[len] != NULL)
		len++;
	return (len);
}

void	free_allocated_memory(char **arr, int curr_index)
{
	int	i;

	i = -1;
	while (++i <= curr_index)
		free(arr[i]);
	free(arr);
	arr = NULL;
}

char	**copy_env(char **envp)
{
	int		len;
	char	**envs;
	int		i;
	int		j;

	len = arr_len(envp);
	envs = (char **)malloc(sizeof(char *) * (len + 1));
	if (envs == NULL)
		print_error(ERR_MALLOC);
	i = -1;
	while (++i < len)
	{
		envs[i] = (char *)malloc(sizeof(char) * ((int)ft_strlen(envp[i]) + 1));
		if (envs[i] == NULL)
		{
			free_allocated_memory(envs, i);
			print_error(ERR_MALLOC);
		}
		j = -1;
		while (++j < (int)ft_strlen(envp[i]))
			envs[i][j] = envp[i][j];
		envs[i][j] = '\0';
	}
	envs[i] = NULL;
	return (envs);
}

void	free_arr(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i] != NULL)
		free(arr[i]);
	free(arr);
	arr = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	int 	pid;
	int 	status;

	(void)argv;
	data.envp = copy_env(envp); // free data.envp before exit
	validate_args(argc);
	data.buf = readline("\033[0;32mLiteShell$ \033[0m"); // free data.buf before exit
	data.builtin = 0;
	while (data.buf != NULL) 
	{
		if (ft_strlen(data.buf) > 0)
			add_history(data.buf);
		// handling buf (parsing + execution)
		// test parsing/execution funcs here!
		// ------
		runcmd(parsecmd(data.buf), &data);
		// ------
		free(data.buf);
		data.buf = readline("\033[0;32mLiteShell$ \033[0m");
	}
	free(data.buf);
	free_arr(data.envp);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}
