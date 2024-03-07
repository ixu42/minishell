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

static int	update_oldpwd_in_env_lst(t_env *env_lst)
{
	char	cwd[PATH_MAX];
	t_env	*tmp;
	t_env	*node;
	char	*name_value_pair;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror(PMT_ERR_GETCWD);
		return (1);
	}
	tmp = env_lst;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->name, "OLDPWD") == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(cwd);
			return (0);
		}
		tmp = tmp->next;
	}
	name_value_pair = ft_strjoin("PWD=", cwd);
	node = get_node(name_value_pair);
	if (node == NULL)
		return (1);
	lst_append(&env_lst, node);
	free(name_value_pair);
	return (0);
}

static int	update_pwd_in_env_lst(t_env *env_lst)
{
	char	cwd[PATH_MAX];
	t_env	*tmp;
	t_env	*node;
	char	*name_value_pair;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror(PMT_ERR_GETCWD);
		return (1);
	}
	tmp = env_lst;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->name, "PWD") == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(cwd);
			return (0);
		}
		tmp = tmp->next;
	}
	name_value_pair = ft_strjoin("PWD=", cwd);
	node = get_node(name_value_pair);
	if (node == NULL)
		return (1);
	lst_append(&env_lst, node);
	free(name_value_pair);
	return (0);
}

int	exec_cd(char **argv, t_env *env_lst)
{
	if (validate_argv(argv) == 1)
		return (1);
	if (update_oldpwd_in_env_lst(env_lst) == 1)
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
		return (update_pwd_in_env_lst(env_lst));
}

int	exec_pwd(char **argv)
{
	char	cwd[PATH_MAX];

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
}
