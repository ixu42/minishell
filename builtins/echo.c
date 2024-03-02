#include "../minishell.h"

static int	print_args(char **argv, int i)
{
	while (argv[i] != NULL)
	{
		if (printf("%s", argv[i]) < 0)
		{
			perror(PMT_ERR_PRINTF);
			return (1);
		}
		if (argv[++i] != NULL)
		{
			if (printf(" ") < 0)
			{
				perror(PMT_ERR_PRINTF);
				return (1);
			}
		}
	}
	return (0);
}

int	exec_echo(char **argv)
{
	int	add_new_line;
	int	start_index;
	int	i;

	add_new_line = 0;
	start_index = 1;
	if (argv[1] != NULL && ft_strcmp(argv[1], "-n") == 0)
		start_index++;
	else
		add_new_line = 1;
	i = start_index;
	if (print_args(argv, i) == 1)
		return (1);
	if (add_new_line)
	{
		if (printf("\n") < 0)
		{
			perror(PMT_ERR_PRINTF);
			return (1);
		}
	}
	return (0);
}
