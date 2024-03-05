#include "../../minishell.h"

static int	validate_argv(char **argv)
{
	if (argv[1] == NULL)
	{
		if (ft_dprintf(2, "%scd: too few arguments\n", PMT) == -1)
			perror(PMT_ERR_WRITE);
		return (1);
	}
	else if (argv[2] != NULL)
	{
		if (ft_dprintf(2, "%scd: too many arguments\n", PMT) == -1)
			perror(PMT_ERR_WRITE);
		return (1);
	}
	return (0);
}

static void	change_pwd_in_env_lst(t_env *env_lst, char *path)
{
	t_env	*tmp;

	tmp = env_lst;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->name, "PWD") == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(path);
			return ;
		}
		tmp = tmp->next;
	}
}

int	exec_cd(char **argv, t_env *env_lst)
{
	if (validate_argv(argv) == 1)
		return (1);
	if (chdir(argv[1]) == -1)
	{
		if (ft_dprintf(2, "%scd: ", PMT) == -1)
		{
			perror(PMT_ERR_WRITE);
			return (1);
		}
		perror(argv[1]);
		return (1);
	}
	else
		change_pwd_in_env_lst(env_lst, get_path(argv[1])); // get path func
	return (0);
}

int	exec_pwd(char **argv, t_env *env_lst)
{
	t_env *tmp;

	tmp = env_lst;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->name, "PWD") == 0)
		{
			if (printf("%s\n", tmp->value) < 0)
			{
				perror(PMT_ERR_PRINTF);
				return (1);
			}
			else
				return (0);
		}
		tmp = tmp->next;
	}	
	return (1);
}

/* int	exec_pwd(char **argv) // malloc char *cwd? work with env lst
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror(PMT_ERR_GETCWD);
		return (1);
	}
	if (printf("%s\n", cwd) < 0)
	{
		perror(PMT_ERR_PRINTF);
		return (1);
	}
	return (0);
} */
