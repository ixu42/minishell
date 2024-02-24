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

void	free_arr(char **arr, int curr_index)
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
			free_arr(envs, i);
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

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argv;
	data.envp = copy_env(envp); // free data.envs before exit
//	runcmd_old(parsecmd(argv[1]), &data);
	//printf("(=%d, |=%d, &=%d, <=%d, >=%d, +=%d\n",'(','|','&','<','>','+');
	//printf("&&=%d, ||=%d\n", AND_TOK, OR_TOK);
	runcmd_test(parsecmd(argv[1]));

	return (0);
}
