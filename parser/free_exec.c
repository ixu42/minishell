/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 10:31:58 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/24 11:39:58 by apimikov         ###   ########.fr       */
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
	/* potencial leakage for cmd with $VAR 
	 if (cmd->type == STR_NODE_VAR)
	 	free(cmd->start);
	 */
	free(cmd);
	return (0);
}

int	free_arg(t_argcmd *cmd)
{
	if (cmd->right)
	{
		free_arg(cmd->right);
		cmd->right = NULL;
	}
	if (cmd->left)
	{
		free_str(cmd->left);
		cmd->left = NULL;
	}
	free(cmd);
	return (0);
}

int	free_exec(t_cmd *cmd)
{
	t_execcmd	*ecmd;

	ecmd = (t_execcmd *)cmd;
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
