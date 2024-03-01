#include "minishell.h"

/* static int	arr_len(char **arr)
{
	int	len;

	len = 0;
	while (arr[len] != NULL)
		len++;
	return (len);
}

char	**copy_env(char **envp)
{
	int		len;
	char	**envs;
	int		i;
	int		j;

	len = arr_len(envp);
	envs = (char **)malloc(sizeof(char *) * (len + 1));
	if (envs == NULL)
		print_error_n_exit(ERR_MALLOC);
	i = -1;
	while (++i < len)
	{
		envs[i] = (char *)malloc(sizeof(char) * ((int)ft_strlen(envp[i]) + 1));
		if (envs[i] == NULL)
		{
			free_arr(envs);
			print_error_n_exit(ERR_MALLOC);
		}
		j = -1;
		while (++j < (int)ft_strlen(envp[i]))
			envs[i][j] = envp[i][j];
		envs[i][j] = '\0';
	}
	envs[i] = NULL;
	return (envs);
} */

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
		int	len = ft_strlen(value);
		if (len > 1)
		{
			if ((value[0] == '"' && value[len - 1] == '"') ||
				(value[0] == '\'' && value[len - 1] == '\''))
				value = remove_quotes(value);
		}
		*equal_sign = '\0';
	}
	else
		value = NULL;
	return (value);
}

t_env	*get_node(char *name_value_str)
{
	t_env	*new_node;
	char 	*name_value_cpy;
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

// copy environment variables from array(char **envp) to linked list(t_envs *envs)

t_env	*copy_env_arr_to_lst(char **envp)
{
	t_env	*head;
	t_env	*new_node;
	t_env	*last_node;
	int		i;

	head = NULL;
	i = -1;
	while (envp[++i] != NULL)
	{
		new_node = get_node(envp[i]);
		lst_append(&head, new_node);
		// if (head == NULL)
		// {
		// 	head = new_node;
		// 	last_node = new_node;
		// }
		// else
		// {
		// 	last_node->next = new_node;
		// 	last_node = new_node;
		// }
	}
	return (head);	
}

size_t	lst_len(t_env *lst)
{
	size_t	len;
	t_env	*tmp;

	len = 0;
	tmp = lst;
	while (tmp != NULL)
	{
		if (tmp->value != NULL)
			len++;
		tmp = tmp->next;
	}
	return (len);
}

char	**copy_env_lst_to_arr(t_env *env_lst) // when value == NULL, is the var copied?
{
	t_env	*tmp;
	char	**env_arr;
	size_t	arr_len;
	size_t	str_len;
	int		i;
	int		j;
	int		k;

	tmp = env_lst;
	arr_len = lst_len(tmp);
	env_arr = (char **)malloc(sizeof(char *) * (arr_len + 1));
	if (env_arr == NULL)
		print_error_n_exit(ERR_MALLOC); // to be updated
	i = 0;
	while (tmp != NULL)
	{
		if (tmp->value != NULL)
		{
			str_len = ft_strlen(tmp->name) + ft_strlen(tmp->value) + 2;
			env_arr[i] = (char *)malloc(sizeof(char) * str_len);
			if (env_arr[i] == NULL)
			{
				free_arr(env_arr); // to be updated
				print_error_n_exit(ERR_MALLOC); // to be updated
			}
			j = -1;
			while (tmp->name[++j] != '\0')
				env_arr[i][j] = tmp->name[j];
			env_arr[i][j] = '=';
			k = -1;
			while (tmp->value[++k] != '\0')
				env_arr[i][++j] = tmp->value[k];
			env_arr[i][++j] = '\0';
			i++;
		}
		tmp = tmp->next;
	}
	env_arr[i] = NULL;
	return (env_arr);
}

// (1) print error message, (2) free data->envp

static void	print_error_partial_free(char *name, t_data *data)
{

	if (ft_putstr_fd("minishell: ", STDERR_FILENO) == -1)
	{
		perror("minishell: write error");
		free_arr(data->envp);
		exit(EXIT_FAILURE);
	}
	perror(name);
	free_arr(data->envp);
	exit(EXIT_FAILURE);
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
