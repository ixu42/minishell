/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 16:10:52 by ixu               #+#    #+#             */
/*   Updated: 2024/03/24 16:16:42 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*join_msgs(char const *s1, char const *s2, char const *s3)
{
	char	*partial_str;
	char	*full_str;

	partial_str = ft_strjoin(s1, s2);
	if (partial_str == NULL)
	{
		perror(PMT_ERR_MALLOC);
		return (NULL);
	}
	full_str = ft_strjoin(partial_str, s3);
	if (full_str == NULL)
	{
		free(partial_str);
		perror(PMT_ERR_MALLOC);
		return (NULL);
	}
	free(partial_str);
	return (full_str);
}

int	handle_exit_or_return(t_data *data, int status_code)
{
	if (data->proc == CHILD_PROC)
	{
		free_data(data);
		data = NULL;
		exit(status_code);
	}
	data->status = status_code;
	return (1);
}
