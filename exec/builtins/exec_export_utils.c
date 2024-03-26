/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_export_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:16:23 by ixu               #+#    #+#             */
/*   Updated: 2024/03/26 11:34:27 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_ascending(char *s1, char *s2)
{
	int	i;

	if (s1 == NULL || s2 == NULL)
		return (1);
	i = 0;
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if (s1[i] < s2[i])
			return (1);
		else if (s1[i] > s2[i])
			return (0);
		i++;
	}
	return (1);
}

static void	swap_name_n_value(t_env *a, t_env *b)
{
	char	*tmp;

	tmp = a->name;
	a->name = b->name;
	b->name = tmp;
	tmp = a->value;
	a->value = b->value;
	b->value = tmp;
}

static t_env	*copy_node(t_env *dest, t_env *src, int *malloc_err)
{
	dest->name = ft_strdup(src->name);
	if (dest->name == NULL)
		return (error_handler(ERR_MALLOC, malloc_err));
	if (src->value == NULL)
		dest->value = NULL;
	else
	{
		dest->value = ft_strdup(src->value);
		if (dest->value == NULL)
			return (error_handler(ERR_MALLOC, malloc_err));
	}
	dest->next = NULL;
	return (dest);
}

static t_env	*lst_cpy(t_env *env_lst, int *malloc_err)
{
	t_env	*tmp;
	t_env	*new_lst;
	t_env	*node;

	tmp = env_lst;
	new_lst = NULL;
	while (tmp != NULL)
	{
		node = (t_env *)malloc(sizeof(t_env));
		if (node == NULL)
			return (error_handler(ERR_MALLOC, malloc_err));
		copy_node(node, tmp, malloc_err);
		if (*malloc_err)
			return (NULL);
		lst_append(&new_lst, node);
		tmp = tmp->next;
	}
	return (new_lst);
}

t_env	*sort_lst(t_env *env_lst, int *malloc_err)
{
	t_env	*env_lst_cpy;
	t_env	*a;
	t_env	*b;

	env_lst_cpy = lst_cpy(env_lst, malloc_err);
	if (*malloc_err)
		return (NULL);
	a = env_lst_cpy;
	if (a->next == NULL)
		return (a);
	while (a->next != NULL)
	{
		b = a->next;
		while (b != NULL)
		{
			if (!is_ascending(a->name, b->name))
				swap_name_n_value(a, b);
			b = b->next;
		}
		a = a->next;
	}
	return (env_lst_cpy);
}
