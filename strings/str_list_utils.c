/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 11:56:25 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/19 12:27:27 by apimikov         ###   ########.fr       */
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
	str->start  = copy;
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

/*
void	expand_var_in_args(t_argcmd *arg, t_data *data)
{
	t_strcmd	*str;
	t_env		*node;

	str = arg->left;
	while (str != NULL)
	{
		if (str->type == STR_NODE_VAR || str->type == STR_NODE_VAR_P)
		{
			if (!name_in_env_lst(data->env_lst, str->start, str->end - str->start, &node))
				str->end = str->start;
			else
				set_variable_node(str, node);
		}
		if (str->type == STR_EXIT_CODE)
			set_exit_code_node(str, data);
		str = str->next;
	}
}
*/

void	expand_var_in_strlist(t_strcmd *str, t_data *data)
{
	t_env		*node;

	while (str != NULL)
	{
		if (str->type == STR_NODE_VAR || str->type == STR_NODE_VAR_P)
		{
			if (!name_in_env_lst(data->env_lst, str->start, str->end - str->start, &node))
				str->end = str->start;
			else
				set_variable_node(str, node);
		}
		if (str->type == STR_EXIT_CODE)
			set_exit_code_node(str, data);
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



void	ft_free_char2d(char **split)
{
	size_t	i;

	if (!split)
		return ;
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

/*
int	get_argc_strcmd(t_strcmd *str)
{
	// to be change when $VAR will be expanded
	return (1);
}
*/



int	get_argc(t_execcmd *cmd)
{
	t_argcmd	*args;
	int			argc;

	argc = 0;
	args = cmd->args;
	while (args != NULL)
	{
		argc++;   //get_argc_strcmd(args->left);
		args = args->right;
	}
	return (argc);
}

char *join_all_arguments(char **pnt)
{
	char	*str;
	size_t	size;
	size_t	i;
	size_t	j;

	size = 0;
	j = 0; 
	while (pnt[j])
		size += ft_strlen(pnt[j++]) + 1;
	//str = (char *)malloc(sizeof(str) * (size + 1));
	if (size == 0)
		size = 1;
	str = (char *)malloc(sizeof(str) * size);
	if (!str)
		return (NULL);
	//ft_memset(str, 0, sizeof(str) * (size + 1));
	ft_memset(str, 0, sizeof(str) * size);
	i = -1;
	j = 0;
	while (++i < size && pnt[j]) 
	{
		ft_strlcpy(str + i, pnt[j], ft_strlen(pnt[j]) + 1);
		i += (size_t)ft_strlen(pnt[j++]);
	    str[i] = ASCII_SEPARATOR;
	}
//	str[size] = '\0';
	str[size - 1] = '\0';
//	ft_dprintf(2,"pnt[0]=->%s<-\n",pnt[0]);
	//ft_dprintf(2,"s=->%s<-\n",str);
	return (str);
}

int	make_argv_expanded(t_execcmd *cmd)
{
	char		*joined_arg;
	int			argc_exp;
	size_t	i;

	//???should we copy cmd->argv for latter freeing
//	printf("argc=%d\n", cmd->argc);
	joined_arg = join_all_arguments(cmd->argv);
	ft_free_char2d(cmd->argv);
	if (!joined_arg)
		return (1);
	cmd->argv = ft_split(joined_arg, ASCII_SEPARATOR);
	free(joined_arg);
	if (!cmd->argv)
		return (1);
	i = 0;
	while (cmd->argv[i])
		i++;
	cmd->argc = i;
	return (0);
}

int	init_argv(t_execcmd *cmd, t_data *data)
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
	while (i <= argc && args != NULL)
	{
//		expand_var_in_args(args, data);
		expand_var_in_strlist(args->left, data);
		argv[i] = strlist_join(args->left);
		if (!argv[i++])
			return (ft_free_char2d_return(argv, 1));
		args = args->right;
	}
	// ??? do we need to clean old value of cmd->argv?
	cmd->argv = argv;
	//data about arraylist is losted ??? clean it arraylist aproprietry
	return (0);
}


int	make_filename(t_redircmd *rcmd, t_data *data)
{
	char	*file;
	int		i;

	if (rcmd->heredoc || rcmd->mode == -1)
		return (0);
	expand_var_in_strlist(rcmd->str, data);
	rcmd->file = strlist_join(rcmd->str);
//	printf("make_filename:->%s<-\n",rcmd->file);
	if (!rcmd->file)
		return (MALLOC_ERROR);
	i = ft_strlen(rcmd->file);
	while (i-- > 0)
	{
		if (rcmd->file[i] == ASCII_SEPARATOR)
		{
			ft_dprintf(2, "%s %s %s\n", PMT, rcmd->sfile, ERR_REDIR_AMBIG);
			return (1);
		}
	}
	if (wildcard_star_redir(rcmd))
		return (1);
	return (0);
}

int	make_argv(t_execcmd *cmd, t_data *data)
{
	if (init_argv(cmd, data))
		return (1);
	if (make_argv_expanded(cmd))
		return (1);
	if (wildcard_star(cmd))
		return (1);
	return (0);
}
