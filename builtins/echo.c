#include "../minishell.h"

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
	while (argv[i] != NULL)
	{
		if (printf("%s", argv[i]) < 0)
		{
			ft_dprintf(2, "%s%s", PMT, ERR_PRINTF);
			return (1);
		}
		i++;
		if (argv[i] != NULL)
		{
			if (printf(" ") < 0)
			{
				ft_dprintf(2, "%s%s", PMT, ERR_PRINTF);
				return (1);
			}
		}
	}
	if (add_new_line)
	{
		if (printf("\n") < 0)
		{
			ft_dprintf(2, "%s%s", PMT, ERR_PRINTF);
			return (1);
		}
	}
	return (0);
}
