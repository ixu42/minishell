#include "../minishell.h"

/* Exit the shell, returning a status of n to the shell’s parent. 
If n is omitted, the exit status is that of the last command executed */

int	exec_exit(char **argv)
{
	if (argv[1] == NULL)
		exit(0);
	else if (argv[2] != NULL)
	{
		ft_dprintf(2, "%sexit: too many arguments\n", PMT); // protect
		return (1);
	}
	else
		exit(ft_atoi(argv[1]));
	return (0);
}
