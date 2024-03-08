#include "../../minishell.h"

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
