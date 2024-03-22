/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 17:31:52 by ixu               #+#    #+#             */
/*   Updated: 2024/03/22 17:33:15 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
				perror(PMT_ERR_PRINTF);
				return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}
