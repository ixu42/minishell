/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 11:55:07 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/24 12:03:34 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_argc(t_execcmd *cmd)
{
	t_argcmd	*args;
	int			argc;

	argc = 0;
	args = cmd->args;
	while (args != NULL)
	{
		argc++;
		args = args->right;
	}
	return (argc);
}

int	ft_strlen_mod(char *str)
{
	int	len;

	len = ft_strlen(str);
	if (len == 0)
		return (1);
	return (len);
}

int	is_arg_with_only_var(t_execcmd *cmd, int i)
{
	int			j;
	t_argcmd	*args;
	t_strcmd	*str;

	j = 0;
	args = cmd->args;
	while (args && j < i)
	{
		j++;
		args = args->right;
	}
	str = args->left;
	while (str)
	{
		if (str->type != STR_NODE_VAR)
			return (0);
		str = str->next;
	}
	return (1);
}

void	write_args_to_str(char *str, t_execcmd *cmd, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	**pnt;

	pnt = cmd->argv;
	i = 0;
	j = 0;
	while (i < size && pnt[j])
	{
		len = ft_strlen(pnt[j]);
		if (len == 0 && is_arg_with_only_var(cmd, j) && ++j)
			continue ;
		if (len == 0)
			ft_strlcpy(str + i++, ASCII_EMPTY_X, 2);
		else
		{
			ft_strlcpy(str + i, pnt[j], len + 1);
			i += (size_t)len;
			str[i] = ASCII_SEPARATOR;
		}
		j++;
		i++;
	}
}

char	*join_all_arguments(char **pnt, t_execcmd *cmd)
{
	char	*str;
	size_t	size;
	size_t	j;

	size = 0;
	j = 0;
	while (pnt[j])
		size += ft_strlen_mod(pnt[j++]) + 1;
	str = (char *)malloc(sizeof(str) * size);
	if (!str)
		return (NULL);
	ft_memset(str, 0, sizeof(str) * size);
	write_args_to_str(str, cmd, size);
	return (str);
}
