/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:14:46 by ixu               #+#    #+#             */
/*   Updated: 2024/03/22 14:05:36 by ixu              ###   ########.fr       */
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

static int	update_oldpwd_in_env_lst(t_data *data)
{
	// char	cwd[PATH_MAX];
	t_env	*tmp;
	t_env	*node;
	char	*name_value_pair;

	// if (getcwd(cwd, sizeof(cwd)) == NULL)
	// {
	// 	perror(PMT_ERR_GETCWD);
	// 	return (1);
	// }
	tmp = data->env_lst;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->name, "OLDPWD") == 0)
		{
			free(tmp->value);
			// dprintf(2, "debug0.1\n");
			// dprintf(2, "data->pwd:%s\n", data->pwd);
			tmp->value = ft_strdup(data->pwd); // protect
			// dprintf(2, "debug0.2\n");
			return (0);
		}
		tmp = tmp->next;
	}
	// dprintf(2, "debug0.3\n");
	// dprintf(2, "data->pwd:%s\n", data->pwd);
	name_value_pair = ft_strjoin("OLDPWD=", data->pwd); // protect
	// dprintf(2, "debug0.4\n");
	node = get_node(name_value_pair);
	if (node == NULL)
		return (1);
	lst_append(&data->env_lst, node);
	free(name_value_pair);
	return (0);
}

static int	update_pwd_in_env_lst(t_data *data)
{
	char	cwd[PATH_MAX];
	t_env	*tmp;
	t_env	*node;
	// char	*name_value_pair;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror(PMT_ERR_GETCWD);
		return (1);
	}
	tmp = data->env_lst;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->name, "PWD") == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(cwd);
			// dprintf(2, "before:data->pwd:%s\n", data->pwd);
			free(data->pwd);
			data->pwd = ft_strdup(cwd);
			// dprintf(2, "after:data->pwd:%s\n", data->pwd);
			return (0);
		}
		tmp = tmp->next;
	}
	// dprintf(2, "before:data->pwd:%s\n", data->pwd);
	// free(data->pwd);
	// data->pwd = ft_strdup(cwd);
	// dprintf(2, "after:data->pwd:%s\n", data->pwd);
	// name_value_pair = ft_strjoin("PWD=", cwd);
	// node = get_node(name_value_pair);
	// if (node == NULL)
	// 	return (1);
	// lst_append(&env_lst, node);
	// free(name_value_pair);
	return (0);
}

int	exec_cd(char **argv, t_data *data)
{
	if (validate_argv(argv) == 1)
		return (1);
	// dprintf(2, "debug0\n");
	if (update_oldpwd_in_env_lst(data) == 1)
		return (1);
	// dprintf(2, "debug1\n");
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
	{
		// dprintf(2, "debug2\n");
		return (update_pwd_in_env_lst(data));
	}
	return (0);
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
