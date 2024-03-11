/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 21:40:39 by ixu               #+#    #+#             */
/*   Updated: 2024/01/14 23:07:00 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	env_paths_init(t_data **data, char **envp)
{
	char	**arr;

	if (*envp == NULL)
	{
		arr = ft_split("/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin", ':');
		if (arr == NULL)
			print_error_partial_free(ERR_MALLOC, *data);
		(*data)->env_paths = arr;
	}
	else
		(*data)->env_paths = get_env_paths(envp, *data);
}

void	data_init(int argc, char **argv, char **envp, t_data **data)
{
	if (ft_strcmp(argv[1], "here_doc") == 0)
		(*data)->here_doc = 1;
	else
		(*data)->here_doc = 0;
	(*data)->proc_num = argc - 3 - (*data)->here_doc;
	(*data)->cmd_arr = NULL;
	(*data)->cmd_path = NULL;
	(*data)->pipe_num = (*data)->proc_num - 1;
	env_paths_init(data, envp);
	(*data)->fds = (int *)malloc(sizeof(int) * (*data)->pipe_num * 2);
	if ((*data)->fds == NULL)
	{
		free_arr((*data)->env_paths);
		free(*data);
		*data = NULL;
		print_error_no_free(ERR_MALLOC);
	}
	(*data)->pids = (int *)malloc(sizeof(int) * (*data)->proc_num);
	if ((*data)->pids == NULL)
		print_error(ERR_MALLOC, *data, EXIT_FAILURE);
}

void	create_pipes(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->pipe_num)
	{
		if (pipe(data->fds + i * 2) == -1)
			print_error(ERR_PIPE, data, EXIT_FAILURE);
	}
}
