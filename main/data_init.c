/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 20:29:16 by ixu               #+#    #+#             */
/*   Updated: 2024/03/25 12:07:44 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* copy environment variables from array(char **envp) 
to linked list(t_envs *envs) */

t_env	*copy_env_arr_to_lst(char **envp)
{
	t_env	*head;
	int		i;

	head = NULL;
	i = -1;
	while (envp[++i] != NULL)
		lst_append_in_init(&head, get_node_in_init(envp[i]));
	return (head);
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

void	increment_shlvl(t_env *env_lst)
{
	t_env	*tmp;
	int		new_shlvl;

	tmp = env_lst;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->name, "SHLVL") == 0 && tmp->value != NULL)
		{
			new_shlvl = ft_atoi(tmp->value) + 1;
			free(tmp->value);
			tmp->value = ft_itoa(new_shlvl);
		}
		tmp = tmp->next;
	}
}

void	data_init(t_data *data, char **envp)
{
	data->builtin = 0;
	data->proc = PARENT_PROC;
	data->status = 0;
	data->under_pipe = 0;
	data->under_redir = 0;
	data->builtin = 0;
	data->status = 0;
	data->fd_stdin = dup(0);
	if (data->fd_stdin == -1)
		print_error_n_exit(ERR_DUP);
	data->fd_stdout = dup(1);
	if (data->fd_stdout == -1)
		print_error_n_exit(ERR_DUP);
	data->envp = NULL;
	data->cmd_path = NULL;
	data->stat_str = NULL;
	data->tree = NULL;
	data->env_lst = copy_env_arr_to_lst(envp);
	increment_shlvl(data->env_lst);
	data->env_paths = get_env_paths(envp, data);
	data->num_heredoc = 0;
}
