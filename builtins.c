#include "minishell.h"

int	exec_echo(char **argv) // the expanded argv
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
			ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0, %s", ERR_PRINTF);
			return (1);
		}
		i++;
		if (argv[i] != NULL)
		{
			if (printf(" ") < 0)
			{
				ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0, %s", ERR_PRINTF);
				return (1);
			}
		}
	}
	if (add_new_line)
	{
		if (printf("\n") < 0)
		{
			ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0, %s", ERR_PRINTF);
			return (1);
		}
	}
	return (0);
}

/* Exit the shell, returning a status of n to the shell’s parent. 
If n is omitted, the exit status is that of the last command executed */

int	exec_exit(char **argv) // the expanded argv
{
	if (argv[1] == NULL) // n is omitted
		exit(0);
	else if (argv[2] != NULL) // too many args
	{
		ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0mexit: too many arguments\n"); // protect
		return (1);
	}
	else
		exit(ft_atoi(argv[1]));
	return (0);
}

int	exec_cd(char **argv)
{
	if (argv[1] == NULL) // If directory is not supplied, the value of the HOME shell variable is used.
	{
		ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0mcd: too few arguments\n"); // protect
		return (1);
	}
	else if (argv[2] != NULL) // too many args
	{
		ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0mcd: too many arguments\n"); // protect
		return (1);
	}
	else
	{
		if (chdir(argv[1]) == -1)
		{
			ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0mcd: ");
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
		ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0, %s", ERR_PRINTF);
		return (1);
	}
	return (0);
}

int	print_exported(t_env *env_lst)
{
	t_env	*tmp;

	tmp = env_lst;
	while (tmp != NULL)
	{
		if (tmp->value == NULL)
			printf("declare -x %s\n", tmp->name);
		else
			printf("declare -x %s=\"%s\"\n", tmp->name, tmp->value);
		// else if (ft_strlen(tmp->value) != 0)
		// 	printf("declare -x %s=\"%s\"\n", tmp->name, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}

/* if the variable already exits in the environment list, 
its value is replaced by the new value, and the func returns 0,
otherwise, the func returns 1 */

int	check_env_lst(char *arg, t_env *env_lst, size_t name_len)
{
	t_env	*tmp;
	char	*value;
	char 	*arg_cpy;

	tmp = env_lst;
	while (tmp != NULL)
	{
		if (ft_strncmp(arg, tmp->name, name_len) == 0)
		{
			arg_cpy = ft_strdup(arg);
			value = get_value(arg_cpy);
			free(arg_cpy);
			if (value != NULL)
			{
				free(tmp->value);
				tmp->value = value;
			}
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

int	exec_export(char **argv, t_env *env_lst)
{
	int		i;
	int		j;
	t_env	*new_node;
	size_t	name_len;

	if (argv[1] == NULL)
		print_exported(env_lst);
	else
	{
		i = 0;
		while (argv[++i] != NULL)
		{
			j = 0;
			name_len = 0;
			while (argv[i][j] != '=' && argv[i][j] != '\0')
			{
				name_len++;
				j++;
			}
			if (check_env_lst(argv[i], env_lst, name_len) != 0)
			{
				new_node = get_node(argv[i]);
				lst_append(&env_lst, new_node);
			}
		}
	}
	return (0);
}

// int	exec_unset(char **argv)
// {
// 	;
// }

int	exec_env(t_env *env_lst)
{
	t_env	*tmp;

	tmp = env_lst;
	while (tmp != NULL)
	{
		if (tmp->value != NULL)
		{
			if (printf("%s=%s\n", tmp->name, tmp->value) < 0)
			{
				ft_dprintf(STDERR_FILENO, "\033[0;31mLiteShell: \033[0, %s", ERR_PRINTF);
				return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}

int	is_builtin(char **argv, t_data **data) // the expanded argv
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

int	run_builtin(char **argv, t_data *data) // the expanded argv
{
	if (data->builtin == ECHO)
		return (exec_echo(argv));
	else if (data->builtin == CD)
		return (exec_cd(argv));
	else if (data->builtin == PWD)
		return (exec_pwd(argv));
	else if (data->builtin == EXPORT)
		return (exec_export(argv, data->env_lst));
	// else if (data->builtin == UNSET)
	// 	return (exec_unset(argv));
	else if (data->builtin == ENV)
		return (exec_env(data->env_lst));
	else if (data->builtin == EXIT)
		return (exec_exit(argv));
}
