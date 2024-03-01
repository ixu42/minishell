#include "minishell.h"

// (1) print error message, (2) exit

void	print_error_n_exit(char *err_msg)
{
	if (ft_putstr_fd("minishell: ", STDERR_FILENO) == -1)
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
		ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0m%s: command not found\n", err_msg); // protect
	else
		ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0m%s\n", err_msg); // protect
	free_data(data);
	// free tree nodes?
	exit(exit_code);
}
