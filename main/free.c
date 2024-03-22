/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 20:29:25 by ixu               #+#    #+#             */
/*   Updated: 2024/03/22 21:00:18 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_arr(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i] != NULL)
		free(arr[i]);
	free(arr);
	arr = NULL;
}

void	free_lst(t_env **lst)
{
	t_env	*current;
	t_env	*prev;

	current = *lst;
	while (current != NULL)
	{
		free(current->name);
		free(current->value);
		prev = current;
		current = current->next;
		free(prev);
	}
	*lst = NULL;
}

// free data struct

void	free_data(t_data *data)
{
	free_lst(&(data->env_lst));
	free_arr(data->env_paths);
	free(data->buf);
}
