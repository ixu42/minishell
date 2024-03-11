/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 11:23:46 by ixu               #+#    #+#             */
/*   Updated: 2024/01/12 11:23:46 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static char	*remove_quotes(char *s)
{
	char	*new_s;
	int		new_len;
	int		i;

	new_len = ft_strlen(s) - 1;
	new_s = (char *)malloc(new_len * sizeof(char));
	if (new_s == NULL)
		return (NULL);
	i = 0;
	while (i < new_len - 1)
	{
		new_s[i] = s[i + 1];
		i++;
	}
	new_s[i] = '\0';
	free(s);
	return (new_s);
}

char	**get_cmd_arr(char *cmd_arg, t_data *data)
{
	char	**cmd_arr;
	int		i;
	int		len;

	cmd_arr = ft_split_pipex(cmd_arg, " 	");
	if (cmd_arr == NULL)
		print_error(ERR_MALLOC, data, EXIT_FAILURE);
	i = 0;
	while (cmd_arr[i] != NULL)
	{
		len = ft_strlen(cmd_arr[i]);
		if (len > 1)
		{
			if ((cmd_arr[i][0] == '"' && cmd_arr[i][len - 1] == '"') ||
				(cmd_arr[i][0] == '\'' && cmd_arr[i][len - 1] == '\''))
				cmd_arr[i] = remove_quotes(cmd_arr[i]);
		}
		i++;
	}
	return (cmd_arr);
}

/* 	verify the validity of the executable/cmd, 
	if the provided name is already with a path */

static char	*verify_path(char *cmd, t_data *data)
{
	if (ft_strchr(cmd, '/') != NULL)
	{
		if (access(cmd, F_OK) != 0)
			print_error(cmd, data, EXIT_CMD_NOT_FOUND);
		if (access(cmd, X_OK) != 0)
			print_error(cmd, data, EXIT_CMD_PERM_ERR);
		return (ft_strdup(cmd));
	}
	return (NULL);
}

char	*get_cmd_path(t_data *data)
{
	char	*cmd_path;
	char	*tmp;
	int		i;

	cmd_path = verify_path(data->cmd_arr[0], data);
	if (cmd_path != NULL)
		return (cmd_path);
	if ((data->env_paths == NULL || ft_strlen(data->env_paths[0]) == 0)
		&& access(data->cmd_arr[0], F_OK | X_OK) == 0)
		return (ft_strdup(data->cmd_arr[0]));
	i = 0;
	while (data->env_paths != NULL && data->env_paths[i] != NULL)
	{
		tmp = ft_strjoin(data->env_paths[i], "/");
		cmd_path = ft_strjoin(tmp, data->cmd_arr[0]);
		free(tmp);
		if (access(cmd_path, F_OK) == 0)
			break ;
		free(cmd_path);
		cmd_path = NULL;
		i++;
	}
	return (cmd_path);
}

char	**get_env_paths(char **envp, t_data *data)
{
	char	*path_line;
	char	**env_paths;
	int		i;

	path_line = NULL;
	while (*envp != NULL)
	{
		if (ft_strnstr(*envp, "PATH=", 5) != NULL)
		{
			path_line = *envp;
			break ;
		}
		envp++;
	}
	if (path_line == NULL)
		return (NULL);
	i = -1;
	while (++i < 5)
		path_line++;
	env_paths = ft_split(path_line, ':');
	if (env_paths == NULL)
		print_error_partial_free(ERR_MALLOC, data);
	return (env_paths);
}
