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

void	data_init(t_data *data, char **envp)
{
	data->envp = NULL;
	data->env_lst = copy_env_arr_to_lst(envp); // free
	// ------ print out list ------
	// t_env	*tmp = data->env_lst;
	// while (tmp != NULL)
	// {
	// 	printf("%s=%s\n", tmp->name, tmp->value);
	// 	tmp = tmp->next;
	// }
	// ----------------------------
	data->env_paths = get_env_paths(envp, data); // free
	data->builtin = 0;
	data->status_code = 0;
}
