/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 10:31:58 by apimikov          #+#    #+#             */
/*   Updated: 2024/04/08 14:17:07 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	free_str(t_strcmd *cmd)
{
	if (cmd->next)
	{
		free_str(cmd->next);
		cmd->next = NULL;
	}
	if (cmd->type == STR_EXIT_CODE || cmd->type == STR_NODE_VAR)
	{
		if (cmd->malloc_flag && cmd->start)
		{
			free(cmd->start);
			cmd->start = NULL;
		}
	}
	free(cmd);
	return (0);
}

int	free_arg(t_argcmd *cmd)
{
	dprintf(2, "in free_arg\n");
	if (cmd->right)
	{
		dprintf(2, "in cmd->right\n");
		free_arg(cmd->right);
		cmd->right = NULL;
	}
	if (cmd->left)
	{
		dprintf(2, "in cmd->left\n");
		free_str(cmd->left);
		dprintf(2, "after free_str\n");
		cmd->left = NULL;
	}
	free(cmd);
	return (0);
}

int	free_exec(t_cmd *cmd)
{
	t_execcmd	*ecmd;

	ecmd = (t_execcmd *)cmd;
	if (ecmd->list == NULL)
		dprintf(2, "ecmd->list is null\n");
	else
		dprintf(2, "something in ecmd->list\n");
	if (ecmd->list)
	{
		free_arrlist(ecmd->list);
		ecmd->list = NULL;
	}
	if (ecmd->args)
	{
		free_arg(ecmd->args);
		ecmd->args = NULL;
	}
	free(cmd);
	return (0);
}
