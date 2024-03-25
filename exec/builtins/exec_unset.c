/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:16:41 by ixu               #+#    #+#             */
/*   Updated: 2024/03/25 15:18:09 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_valid_identifier(char *name, int name_len)
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

static void	unset_var(t_env *env_lst, t_env *node)
{
	t_env	*tmp;

	tmp = env_lst;
	while (tmp->next != node)
		tmp = tmp->next;
	tmp->next = node->next;
	free(node->name);
	free(node->value);
	free(node);
	node = NULL;
}

int	exec_unset(char **argv, t_data *data)
{
	int		i;
	t_env	*node;
	int		status;

	i = 0;
	status = 0;
	while (argv[++i] != NULL)
	{
		if (!is_valid_identifier(argv[i], ft_strlen(argv[i])))
		{
			if (ft_dprintf(2, "%sunset: '%s': %s\n", PMT, argv[i], ERR_ID) < 0)
				return (perror_n_return(PMT_ERR_WRITE, 1));
			status = 1;
			continue ;
		}
		if (name_in_env_lst(data->env_lst, argv[i], ft_strlen(argv[i]), &node))
			unset_var(data->env_lst, node);
		if (ft_strcmp(argv[i], "PATH") == 0)
		{
			free_arr(data->env_paths);
			data->env_paths = NULL;
		}
	}
	return (status);
}
