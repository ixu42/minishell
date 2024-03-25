/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:16:41 by ixu               #+#    #+#             */
/*   Updated: 2024/03/25 13:20:48 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

	i = 0;
	while (argv[++i] != NULL)
	{
		if (!is_valid_identifier(argv[i], ft_strlen(argv[i])))
		{
			if (ft_dprintf(2, "%sunset: '%s': %s\n", PMT, argv[i], ERR_ID) < 0)
				perror(PMT_ERR_WRITE);
			return (1);
		}
		if (name_in_env_lst(data->env_lst, argv[i], ft_strlen(argv[i]), &node))
			unset_var(data->env_lst, node);
		if (ft_strcmp(argv[i], "PATH") == 0)
		{
			free_arr(data->env_paths);
			data->env_paths = NULL;
		}
	}
	return (0);
}
