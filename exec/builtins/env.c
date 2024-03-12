#include "../../minishell.h"

char	*get_value(char *name_value_str, t_env *new_node, int *err_flag)
{
	char	*equal_sign;
	char	*value;

	equal_sign = ft_strchr(name_value_str, '=');
	if (equal_sign != NULL)
	{
		value = ft_strdup(equal_sign + 1);
		if (value == NULL)
		{
			free(name_value_str);
			free(new_node);
			return ((char *)error_handler(ERR_MALLOC, err_flag));
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
	char	*name_value_cpy;
	int		err_flag;

	name_value_cpy = ft_strdup(name_value_str);
	if (name_value_cpy == NULL)
		return (error_handler(ERR_MALLOC, &err_flag));
	new_node = (t_env *)malloc(sizeof(t_env));
	if (new_node == NULL)
	{
		free(name_value_cpy);
		return (error_handler(ERR_MALLOC, &err_flag));
	}
	new_node->value = get_value(name_value_cpy, new_node, &err_flag);
	if (err_flag == 1)
		return (NULL);
	new_node->name = ft_strdup(name_value_cpy);
	if (new_node->name == NULL)
	{
		free(name_value_cpy);
		free(new_node->value);
		free(new_node);
		return (error_handler(ERR_MALLOC, &err_flag));
	}
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

int	name_in_env_lst(t_env *env_lst, char *arg, size_t name_len, t_env **node)
{
	t_env	*tmp;

	tmp = env_lst;
	while (tmp != NULL)
	{
		if (name_len == ft_strlen(tmp->name)
			&& ft_strncmp(tmp->name, arg, name_len) == 0)
		{
			*node = tmp;
			return (1);
		}
		tmp = tmp->next;
	}
	*node = NULL;
	return (0);
}

int	exec_env(t_env *env_lst)
{
	t_env	*tmp;

	tmp = env_lst;
	while (tmp != NULL)
	{
		if (tmp->value != NULL)
		{
			if (printf("%s=%s\n", tmp->name, tmp->value) < 0)
			{
				perror(PMT_ERR_PRINTF);
				return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}
