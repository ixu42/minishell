#include "../minishell.h"

void	validate_args(int argc)
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

// (1) print error message, (2) exit

void	print_error_n_exit(char *err_msg)
{
	if (ft_putstr_fd("minishell: ", 2) == -1)
	{
		perror("minishell: write error");
		exit(EXIT_FAILURE);
	}
	perror(err_msg);
	exit(EXIT_FAILURE);
}

// (1) print error message, (2) free all heap allocated memory, (3) exit with status code

void	panic(char *err_msg, t_data *data, int exit_code)
{
	if (exit_code == EXIT_CMD_NOT_FOUND)
		ft_dprintf(2, "%s%s: command not found\n", PMT, err_msg); // protect
	else
		ft_dprintf(2, "%s%s\n", PMT, err_msg); // protect
	free_data(data);
	// free tree nodes?
	exit(exit_code);
}
