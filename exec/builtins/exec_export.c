/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:16:23 by ixu               #+#    #+#             */
/*   Updated: 2024/03/22 20:02:25 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	print_exported(t_env *env_lst)
{
	t_env	*tmp;

	tmp = env_lst;
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
	return (0);
}

static int	is_valid_identifier(char *name, int name_len)
{
	int	i;

	if (name == NULL || name_len == 0)
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 0;
	while (++i < name_len)
	{
		if (!ft_isalpha(name[i]) && !ft_isdigit(name[i]) && name[i] != '_')
			return (0);
	}
	return (1);
}

static int	set_value(char *arg, t_env *node)
{
	t_env	*tmp;
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
	}
	return (0);
}

static int	export(char *arg, t_env *env_lst, t_env *node, size_t name_len)
{
	if (name_in_env_lst(env_lst, arg, name_len, &node))
	{
		if (set_value(arg, node) == 1)
			return (1);
	}
	else
	{
		node = get_node(arg);
		if (node == NULL)
			return (1);
		lst_append(&env_lst, node);
	}
	return (0);
}

int	exec_export(char **argv, t_env *env_lst)
{
	int		i;
	int		j;
	t_env	*node;
	size_t	name_len;

	if (argv[1] == NULL)
		print_exported(env_lst);
	i = 0;
	while (argv[++i] != NULL)
	{
		j = -1;
		name_len = 0;
		while (argv[i][++j] != '=' && argv[i][j] != '\0')
			name_len++;
		if (!is_valid_identifier(argv[i], name_len))
		{
			if (ft_dprintf(2, "%sexport: '%s': %s\n", PMT, argv[i], ERR_ID) < 0)
				perror(PMT_ERR_WRITE);
			return (1);
		}
		if (export(argv[i], env_lst, node, name_len) == 1)
			return (1);
	}
	return (0);
}
