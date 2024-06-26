/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructors_tree.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 10:31:09 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/26 11:25:25 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*execcmd(void)
{
	t_execcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = EXEC;
	cmd->argc = 0;
	cmd->list = NULL;
	return ((t_cmd *)cmd);
}

t_cmd	*redircmd(t_cmd *subcmd, t_strstate *state, int mode, int fd)
{
	t_redircmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
	{
		subcmd->flag |= MALLOC_ERROR;
		state->flag |= MALLOC_ERROR;
		return (NULL);
	}
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = REDIR;
	cmd->cmd = subcmd;
	cmd->file = state->beg;
	cmd->sfile = state->beg;
	cmd->efile = state->end;
	cmd->mode = mode;
	cmd->fd = fd;
	cmd->str = NULL;
	cmd->list = NULL;
	cmd->heredoc = state->heredoc;
	if (cmd->heredoc)
		cmd->file = cmd->heredoc;
	return ((t_cmd *)cmd);
}

t_cmd	*pipecmd(t_cmd *left, t_cmd *right)
{
	t_pipecmd	*cmd;

	if (!left)
		return (NULL);
	if (!right)
	{
		freecmd(left);
		return (NULL);
	}
	cmd = malloc(sizeof(*cmd));
	if (!cmd)
	{
		freecmd(left);
		freecmd(right);
		return (NULL);
	}
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	cmd->flag = left->flag | right->flag;
	return ((t_cmd *)cmd);
}

t_cmd	*list_cmd(t_cmd *left, t_cmd *right, int type)
{
	t_listcmd	*cmd;

	if (!left)
		return (NULL);
	if (!right)
	{
		freecmd(left);
		return (NULL);
	}
	cmd = malloc(sizeof(*cmd));
	if (!cmd)
	{
		freecmd(left);
		freecmd(right);
		return (NULL);
	}
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = type;
	cmd->left = left;
	cmd->right = right;
	cmd->flag = left->flag;
	cmd->flag |= right->flag;
	return ((t_cmd *)cmd);
}
