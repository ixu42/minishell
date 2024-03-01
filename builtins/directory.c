#include "../minishell.h"

int	exec_cd(char **argv)
{
	if (argv[1] == NULL)
	{
		ft_dprintf(2, "%scd: too few arguments\n", PMT); // protect
		return (1);
	}
	else if (argv[2] != NULL)
	{
		ft_dprintf(2, "%scd: too many arguments\n", PMT); // protect
		return (1);
	}
	else
	{
		if (chdir(argv[1]) == -1)
		{
			ft_dprintf(2, "%scd: ", PMT);
			perror(argv[1]);
			return (1);
		}
	}
	return (0);
}

int	exec_pwd(char **argv)
{
	char	cwd[1024]; // malloc?

	if (getcwd(cwd, sizeof(cwd)) == NULL) // error msg?
		return (1);
	if (printf("%s\n", cwd) < 0)
	{
		ft_dprintf(2, "%s%s\n", PMT, ERR_PRINTF); // protect
		return (1);
	}
	return (0);
}
