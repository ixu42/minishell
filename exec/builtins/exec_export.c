/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:16:23 by ixu               #+#    #+#             */
/*   Updated: 2024/03/26 11:34:36 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	print_exported(t_env *env_lst)
{
	t_env	*env_lst_cpy;
	int		malloc_err;
	t_env	*tmp;

	malloc_err = 0;
	env_lst_cpy = sort_lst(env_lst, &malloc_err);
	if (malloc_err)
		return (1);
	tmp = env_lst_cpy;
	while (tmp != NULL)
	{
		if (tmp->value == NULL)
		{
			if (printf("declare -x %s\n", tmp->name) < 0)
				perror(PMT_ERR_PRINTF);
		}
		else
		{
			if (printf("declare -x %s=\"%s\"\n", tmp->name, tmp->value) < 0)
				perror(PMT_ERR_PRINTF);
		}
		tmp = tmp->next;
	}
	free_lst(&env_lst_cpy);
	return (0);
}

static int	get_name_len(char *name_value_pair)
{
	size_t	name_len;
	int		j;

	name_len = 0;
	j = -1;
	while (name_value_pair[++j] != '=' && name_value_pair[j] != '\0')
		name_len++;
	return (name_len);
}

static int	set_value(char *arg, t_env *node, t_data *data)
{
	char	*arg_cpy;
	char	*value;
	int		err_flag;

	arg_cpy = ft_strdup(arg);
	value = get_value(arg_cpy, node, &err_flag);
	if (err_flag == 1)
		return (1);
	free(arg_cpy);
	if (value != NULL)
	{
		free(node->value);
		node->value = value;
		if (ft_strcmp(node->name, "PATH") == 0)
		{
			free_arr(data->env_paths);
			data->env_paths = ft_split(value, ':');
			if (data->env_paths == NULL)
				return (perror_n_return(PMT_ERR_MALLOC, 1));
		}
	}
	return (0);
}

static int	export(char *arg, t_data *data, size_t name_len)
{
	t_env	*node;

	if (name_in_env_lst(data->env_lst, arg, name_len, &node))
	{
		if (set_value(arg, node, data) == 1)
			return (1);
	}
	else
	{
		node = get_node(arg);
		if (node == NULL)
			return (1);
		lst_append(&(data->env_lst), node);
	}
	return (0);
}

int	exec_export(char **argv, t_data *data)
{
	int		i;
	size_t	name_len;
	int		status;

	if (argv[1] == NULL)
	{
		if (print_exported(data->env_lst) == 1)
			return (1);
	}
	i = 0;
	status = 0;
	while (argv[++i] != NULL)
	{
		name_len = get_name_len(argv[i]);
		if (!is_valid_identifier(argv[i], name_len))
		{
			if (ft_dprintf(2, "%sexport: '%s': %s\n", PMT, argv[i], ERR_ID) < 0)
				return (perror_n_return(PMT_ERR_WRITE, 1));
			status = 1;
			continue ;
		}
		if (export(argv[i], data, name_len) == 1)
			return (1);
	}
	return (status);
}
