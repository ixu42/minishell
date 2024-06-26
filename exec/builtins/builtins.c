/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:14:36 by ixu               #+#    #+#             */
/*   Updated: 2024/03/23 14:18:50 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_builtin(char **argv, t_data **data)
{
	if (ft_strcmp(argv[0], "echo") == 0)
		(*data)->builtin = ECHO_CMD;
	else if (ft_strcmp(argv[0], "cd") == 0)
		(*data)->builtin = CD_CMD;
	else if (ft_strcmp(argv[0], "pwd") == 0)
		(*data)->builtin = PWD_CMD;
	else if (ft_strcmp(argv[0], "export") == 0)
		(*data)->builtin = EXPORT_CMD;
	else if (ft_strcmp(argv[0], "unset") == 0)
		(*data)->builtin = UNSET_CMD;
	else if (ft_strcmp(argv[0], "env") == 0)
		(*data)->builtin = ENV_CMD;
	else if (ft_strcmp(argv[0], "exit") == 0)
		(*data)->builtin = EXIT_CMD;
	else
		return (0);
	return (1);
}

int	run_builtin(char **argv, t_data *data)
{
	if (data->builtin == ECHO_CMD)
		return (exec_echo(argv));
	else if (data->builtin == CD_CMD)
		return (exec_cd(argv, data));
	else if (data->builtin == PWD_CMD)
		return (exec_pwd());
	else if (data->builtin == EXPORT_CMD)
		return (exec_export(argv, data));
	else if (data->builtin == UNSET_CMD)
		return (exec_unset(argv, data));
	else if (data->builtin == ENV_CMD)
		return (exec_env(data->env_lst));
	else if (data->builtin == EXIT_CMD)
		return (exec_exit(argv, data));
	return (1);
}
