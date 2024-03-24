/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 20:29:25 by ixu               #+#    #+#             */
/*   Updated: 2024/03/24 17:32:54 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_arr(char **arr)
{
	int	i;

	if (arr == NULL)
		return ;
	i = -1;
	while (arr[++i] != NULL)
		free(arr[i]);
	free(arr);
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
	freecmd_null(&(data->tree));
	if (data->stat_str)
		free(data->stat_str);
	free(data->cmd_path);
	free_arr(data->envp);
}

// used during the execution

void	free_n_exit(t_data *data, int status_code)
{
	free_data(data);
	exit(status_code);
}
