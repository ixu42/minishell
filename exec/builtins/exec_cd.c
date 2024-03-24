/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:14:46 by ixu               #+#    #+#             */
/*   Updated: 2024/03/23 13:52:55 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static int	set_pwd_env_values(t_data *data, char *var, char cwd[PATH_MAX])
{
	t_env	*tmp;

	tmp = data->env_lst;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->name, var) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(cwd);
			if (tmp->value == NULL)
			{
				perror(PMT_ERR_MALLOC);
				return (1);
			}
			return (0);
		}
		tmp = tmp->next;
	}
	return (0);
}

static int	update_oldpwd_in_env_lst(t_data *data)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror(PMT_ERR_GETCWD);
		return (1);
	}
	if (set_pwd_env_values(data, "OLDPWD", cwd) == 1)
		return (1);
	return (0);
}

static int	update_pwd_in_env_lst(t_data *data)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror(PMT_ERR_GETCWD);
		return (1);
	}
	if (set_pwd_env_values(data, "PWD", cwd) == 1)
		return (1);
	return (0);
}

int	exec_cd(char **argv, t_data *data)
{
	if (validate_argv(argv) == 1)
		return (1);
	if (update_oldpwd_in_env_lst(data) == 1)
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
		return (update_pwd_in_env_lst(data));
	return (0);
}
