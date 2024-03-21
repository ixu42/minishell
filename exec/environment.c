/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:16:56 by ixu               #+#    #+#             */
/*   Updated: 2024/03/21 15:16:57 by ixu              ###   ########.fr       */
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

char	**copy_env_lst_to_arr(t_env *env_lst)
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
