/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 12:00:51 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/24 12:02:46 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_exit_code_node(t_strcmd *str, t_data *data)
{
	if (data->stat_str)
		free(data->stat_str);
	data->stat_str = ft_itoa(data->status);
	str->start = data->stat_str;
	str->end = str->start + ft_strlen(str->start);
}

void	replace_space_by(t_strcmd *str, int c)
{
	char	*s;

	s = str->start;
	while (s < str->end)
	{
		if (ft_strchr(WHITESPACE, *s))
			*s = (char)c;
		s++;
	}
}

void	duplicate_data_in_str(t_strcmd *str)
{
	char	*copy;

	copy = ft_strdup(str->start);
	str->start = copy;
	str->end = copy + ft_strlen(copy);
}

void	set_variable_node(t_strcmd *str, t_env *node)
{
	str->start = node->value;
	str->end = node->value;
	while (*str->end)
		str->end++;
	if (str->type == STR_NODE_VAR_P)
		str->type = STR_NODE;
	else if (str->type == STR_NODE_VAR)
	{
		duplicate_data_in_str(str);
		replace_space_by(str, ASCII_SEPARATOR);
	}
}

void	expand_var_in_strlist(t_strcmd *str, t_data *data)
{
	t_env		*node;

	while (str != NULL)
	{
		if (str->type == STR_NODE_VAR || str->type == STR_NODE_VAR_P)
		{
			if (!name_in_env_lst(data->env_lst, str->start, \
				str->end - str->start, &node))
				str->end = str->start;
			else
				set_variable_node(str, node);
		}
		if (str->type == STR_EXIT_CODE)
			set_exit_code_node(str, data);
		str = str->next;
	}
}
