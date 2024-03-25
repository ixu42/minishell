/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_argv_and_filename.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 11:45:48 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/25 11:56:57 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	make_argv_expanded(t_execcmd *cmd)
{
	char		*joined_arg;

	joined_arg = join_all_arguments(cmd->argv, cmd);
	ft_free_char2d(cmd->argv);
	cmd->argv = NULL;
	cmd->argc = 0;
	if (!joined_arg)
		return (1);
	if (!*joined_arg)
	{
		free(joined_arg);
		return (0);
	}
	cmd->argv = ft_split(joined_arg, ASCII_SEPARATOR);
	free(joined_arg);
	if (!cmd->argv)
		return (1);
	while (cmd->argv[cmd->argc])
	{
		if (cmd->argv[cmd->argc][0] == ASCII_EMPTY)
			cmd->argv[cmd->argc][0] = '\0';
		cmd->argc++;
	}
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
		expand_var_in_strlist(args->left, data);
		argv[i] = strlist_join(args->left);
		if (!argv[i++])
			return (ft_free_char2d_return(argv, 1));
		args = args->right;
	}
	cmd->argv = argv;
	return (0);
}

int	is_str_with_only_var(t_strcmd *str)
{
	while (str)
	{
		if (str->type != STR_NODE_VAR)
			return (0);
		str = str->next;
	}
	return (1);
}

int	make_filename(t_redircmd *rcmd, t_data *data)
{
	int	i;

	if (rcmd->heredoc || rcmd->mode == -1)
		return (0);
	expand_var_in_strlist(rcmd->str, data);
	rcmd->file = strlist_join(rcmd->str);
	if (!rcmd->file)
		return (MALLOC_ERROR);
	i = ft_strlen(rcmd->file);
	while (i-- >= 0)
	{
		if (rcmd->file[i] == ASCII_SEPARATOR || \
				(rcmd->file[0] == '\0' && is_str_with_only_var(rcmd->str)))
		{
			ft_dprintf(2, "%s %s %s\n", PMT, rcmd->sfile, ERR_REDIR_AMBIG);
			free(rcmd->file);
			return (1);
		}
	}
	return (wildcard_star_redir(rcmd));
}

int	make_argv(t_execcmd *cmd, t_data *data)
{
	if (init_argv(cmd, data))
		return (1);
	if (make_argv_expanded(cmd))
		return (1);
	if (wildcard_star(cmd) && cmd->argc)
		return (1);
	return (0);
}
