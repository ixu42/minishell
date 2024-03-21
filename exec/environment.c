/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:16:56 by ixu               #+#    #+#             */
/*   Updated: 2024/03/21 23:01:26 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	lst_len(t_env *lst)
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

static void	copy_to_str(char *s, t_env *node)
{
	int	j;
	int	k;

	j = -1;
	while (node->name[++j] != '\0')
		s[j] = node->name[j];
	s[j] = '=';
	k = -1;
	while (node->value[++k] != '\0')
		s[++j] = node->value[k];
	s[++j] = '\0';
}

static void	copy_lst_to_arr(t_env *env_lst, char **env_arr, t_data *data)
{
	t_env	*tmp;
	size_t	str_len;
	int		i;

	i = 0;
	tmp = env_lst;
	while (tmp != NULL)
	{
		if (tmp->value != NULL)
		{
			str_len = ft_strlen(tmp->name) + ft_strlen(tmp->value) + 2;
			env_arr[i] = (char *)malloc(sizeof(char) * str_len);
			if (env_arr[i] == NULL)
			{
				free_arr(env_arr);
				panic(ERR_MALLOC, data, 1);
			}
			copy_to_str(env_arr[i], tmp);
			i++;
		}
		tmp = tmp->next;
	}
	env_arr[i] = NULL;
}

char	**copy_env_lst_to_arr(t_env *env_lst, t_data *data)
{
	char	**env_arr;
	size_t	arr_len;

	arr_len = lst_len(env_lst);
	env_arr = (char **)malloc(sizeof(char *) * (arr_len + 1));
	if (env_arr == NULL)
		panic(ERR_MALLOC, data, 1);
	copy_lst_to_arr(env_lst, env_arr, data);
	return (env_arr);
}
