#include "../minishell.h"

char	*get_value(char *name_value_str)
{
	char	*equal_sign;
	char	*value;

	equal_sign = ft_strchr(name_value_str, '=');
	if (equal_sign != NULL)
	{
		value = ft_strdup(equal_sign + 1);
		if (value == NULL)
			print_error_n_exit(ERR_MALLOC); // to be updated
		*equal_sign = '\0';
	}
	else
		value = NULL;
	return (value);
}

t_env	*get_node(char *name_value_str)
{
	t_env	*new_node;
	char	*name_value_cpy;
	int		i;

	name_value_cpy = ft_strdup(name_value_str);
	if (name_value_cpy == NULL)
		print_error_n_exit(ERR_MALLOC); // to be updated
	new_node = (t_env *)malloc(sizeof(t_env));
	if (new_node == NULL)
		print_error_n_exit(ERR_MALLOC); // to be updated
	new_node->value = get_value(name_value_cpy);
	new_node->name = ft_strdup(name_value_cpy);
	if (new_node->name == NULL)
		print_error_n_exit(ERR_MALLOC); // to be updated
	new_node->next = NULL;
	free(name_value_cpy);
	return (new_node);
}

void	lst_append(t_env **env_lst, t_env *new_node)
{
	t_env	*last_node;

	if (*env_lst == NULL)
	{
		*env_lst = new_node;
		return ;
	}
	last_node = *env_lst;
	while (last_node->next != NULL)
		last_node = last_node->next;
	last_node->next = new_node;
}

// (1) print error message, (2) free data->envp

void	print_error_partial_free(char *name, t_data *data)
{

	if (ft_putstr_fd("minishell: ", 2) == -1)
	{
		perror("minishell: write error");
		free_arr(data->envp);
		exit(EXIT_FAILURE);
	}
	perror(name);
	free_arr(data->envp);
	exit(EXIT_FAILURE);
}
