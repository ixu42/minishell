#include "../../minishell.h"

static void	unset_var(t_env *env_lst, t_env *node)
{
	t_env	*tmp;

	tmp = env_lst;
	while (tmp->next != node)
		tmp = tmp->next;
	tmp->next = node->next;
	free(node->name);
	free(node->value);
	free(node);
	node = NULL;
}

int	exec_unset(char **argv, t_env *env_lst)
{
	int		i;
	t_env	*node;

	i = 0;
	while (argv[++i] != NULL)
	{
		if (name_in_env_lst(env_lst, argv[i], ft_strlen(argv[i]), &node))
			unset_var(env_lst, node);
	}
	return (0);
}
