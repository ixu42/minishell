#include "minishell.h"

void	free_arr(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i] != NULL)
		free(arr[i]);
	free(arr);
	arr = NULL;
}

// free data struct

void	free_data(t_data *data)
{
	free_arr(data->envp);
	free_arr(data->env_paths);
	free(data->buf);
}
