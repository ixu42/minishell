/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:14:11 by ixu               #+#    #+#             */
/*   Updated: 2024/03/25 10:10:07 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	has_arg(t_execcmd *ecmd, t_data *data)
{
	if (ecmd->argc == 0)
	{
		if (restore_stdin_n_stdout(data) == 1)
			return (0);
		if (data->proc == CHILD_PROC)
			free_n_exit(data, 0);
		return (0);
	}
	return (1);
}

static void	execute(t_execcmd *ecmd, t_data *data)
{
	if (ecmd->argc == 1 && ecmd->argv != NULL && \
		ft_strcmp(ecmd->argv[0], "./minishell") == 0)
		increment_shlvl(data->env_lst);
	data->cmd_path = get_cmd_path(ecmd->argv, data);
	data->envp = copy_env_lst_to_arr(data->env_lst, data);
	if (data->cmd_path != NULL)
		execve(data->cmd_path, ecmd->argv, data->envp);
	panic_cmd_not_found(ecmd->argv[0], data);
}

static int	make_child_proc_n_exec(t_execcmd *ecmd, t_data *data)
{
	pid_t	pid;
	int		status;

	pid = fork1(data);
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		data->proc = CHILD_PROC;
		execute(ecmd, data);
	}
	if (ignore_signals() == 1)
		return (panic(ERR_SIGACTION, data, 1));
	if (restore_stdin_n_stdout(data) == 1)
		return (1);
	if (waitpid(pid, &status, 0) == -1)
		return (panic(ERR_WAITPID, data, 1));
	if (WIFEXITED(status))
		data->status = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		data->status = 128 + WTERMSIG(status);
	return (0);
}

void	run_exec(t_cmd *cmd, t_data *data)
{
	t_execcmd	*ecmd;

	ecmd = (t_execcmd *)cmd;
	if (ecmd->argc)
		make_argv(ecmd, data);
	if (!has_arg(ecmd, data))
		return ;
	if (is_builtin(ecmd->argv, &data))
	{
		data->status = run_builtin(ecmd->argv, data);
		if (restore_stdin_n_stdout(data) == 1)
			return ;
		if (data->proc == CHILD_PROC)
			free_n_exit(data, data->status);
	}
	else
	{
		if (data->proc == CHILD_PROC)
			execute(ecmd, data);
		else
			make_child_proc_n_exec(ecmd, data);
	}
}
