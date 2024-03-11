/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 11:56:25 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/08 13:18:02 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
/*
void	expand_var_node(t_strcmd *str, t_env *env_list)
{
	char	*end;
  t_env	*node;

	if (!(str->type == STR_NODE_VAR || str->type == STR_NODE_VAR_P))
		return ;
	printf("expand var?\n");
	if (!name_in_env_lst(env_list, str->start, str->end - str->start, &node))
	{
		printf("expand var? type =%d\n", str->type);
		str->end = str->start;
		return ;
	}
	printf("hi I am var\n");
	str->start = node->value;
	end = node->value;
	while (*end)
		end++;
	str->end = end;
}
*/

void	expand_var_in_args(t_argcmd *arg, t_env *env_list)
{
	t_strcmd	*str;
  t_env			*node;

	str = arg->left;
	while (str != NULL)
	{
		if (str->type == STR_NODE_VAR || str->type == STR_NODE_VAR_P)
		{
			if (!name_in_env_lst(env_list, str->start, str->end - str->start, &node))
				str->end = str->start;
			else
			{
				str->start = node->value;
				str->end = node->value;
				str->type = STR_NODE;
				while (*str->end)
					str->end++;
			}
		}
		str = str->next;
	}
}

int	strlist_len(t_strcmd *str)
{
	int	i;

	i = 0;
	while (str != NULL)
	{
		i += str->end - str->start;
		str = str->next;
	}
	return (i);
}

char	*strlist_join(t_strcmd *str)
{
	char	*res;
	int		len;
	int		i;
	int		j;

	len = strlist_len(str);
	res = (char *)malloc(sizeof(*res) * (len + 1));
	if (!res)
		return (NULL);
	ft_memset(res, 0, sizeof(*res) * (len + 1));
	i = 0;
	while (str != NULL && i < len)
	{
		j = 0;
		while (j < str->end - str->start && str->start[j] != '\0')
			res[i++] = str->start[j++];
		str = str->next;
	}
	return (res);
}

/*
void	replace_var_nodes(t_strcmd *str, char *data)
{
	while (str !=NULL)
	{
		
		str = str->next;
	}
}
*/

void	ft_free_char2d(char **split)
{
	size_t	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

int	ft_free_char2d_return(char **split, int ret)
{
	ft_free_char2d(split);
	return (ret);
}

int	get_argc_strcmd(t_strcmd *str)
{
	// to be change when $VAR will be expanded
	return (1);
}

int	get_argc(t_execcmd *cmd)
{
	t_argcmd	*args;
	int			argc;

	argc = 0;
	args = cmd->args;
	while (args != NULL)
	{
		argc += get_argc_strcmd(args->left);
		args = args->right;
	}
	return (argc);
}

int	make_argv(t_execcmd *cmd, t_data *data)
{
	t_argcmd	*args;
	char		**argv;
	int			argc;
	int			i;

	args = cmd->args;
	argc = get_argc(cmd);
	argv = (char **)malloc(sizeof(*argv) * (argc + 1));
	if (!argv)
		return (1);
	ft_memset(argv, 0, sizeof(*argv) * (argc + 1));
	i = 0;
	while (i <=  argc && args != NULL)
	{
		expand_var_in_args(args, data->env_lst);
		argv[i] = strlist_join(args->left);
		if (!argv[i++])
			return (ft_free_char2d_return(argv, 1));
		args = args->right;
	}
	cmd->argv = argv;
	return (0);
}
