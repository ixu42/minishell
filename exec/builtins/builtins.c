#include "../../minishell.h"

int	is_builtin(char **argv, t_data **data)
{
	if (ft_strcmp(argv[0], "echo") == 0)
		(*data)->builtin = ECHO;
	else if (ft_strcmp(argv[0], "cd") == 0)
		(*data)->builtin = CD;
	else if (ft_strcmp(argv[0], "pwd") == 0)
		(*data)->builtin = PWD;
	else if (ft_strcmp(argv[0], "export") == 0)
		(*data)->builtin = EXPORT;
	else if (ft_strcmp(argv[0], "unset") == 0)
		(*data)->builtin = UNSET;
	else if (ft_strcmp(argv[0], "env") == 0)
		(*data)->builtin = ENV;
	else if (ft_strcmp(argv[0], "exit") == 0)
		(*data)->builtin = EXIT;
	else
		return (0);
	return (1);
}

int	run_builtin(char **argv, t_data *data)
{
	if (data->builtin == ECHO)
		return (exec_echo(argv));
	else if (data->builtin == CD)
		return (exec_cd(argv, data->env_lst));
	else if (data->builtin == PWD)
		return (exec_pwd(argv, data->env_lst));
	else if (data->builtin == EXPORT)
		return (exec_export(argv, data->env_lst));
	else if (data->builtin == UNSET)
		return (exec_unset(argv, data->env_lst));
	else if (data->builtin == ENV)
		return (exec_env(data->env_lst));
	else if (data->builtin == EXIT)
		return (exec_exit(argv));
	return (1);
}
