/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nulterminate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 03:59:46 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/25 08:17:34 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	nulterminate_exec(t_cmd *cmd)
{
	t_execcmd	*ecmd;
	int			i;

	ecmd = (t_execcmd *)cmd;
	i = 0;
	while (ecmd->sargv[i])
		*ecmd->eargv[i++] = 0;
}

void	nulterminate_pipe(t_cmd *cmd)
{
	t_redircmd	*rcmd;

	rcmd = (t_redircmd *)cmd;
	nulterminate(rcmd->cmd);
	*rcmd->efile = 0;
}

t_cmd	*nulterminate(t_cmd *cmd)
{
	t_listcmd	*lcmd;
	t_pipecmd	*pcmd;

	if (cmd == 0)
		return (NULL);
	else if (cmd->type == EXEC)
		nulterminate_exec(cmd);
	else if (cmd->type == REDIR)
		nulterminate_pipe(cmd);
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		nulterminate(pcmd->left);
		nulterminate(pcmd->right);
	}
	else if (cmd->type == AND_CMD || cmd->type == OR_CMD)
	{
		lcmd = (t_listcmd *)cmd;
		nulterminate(lcmd->left);
		nulterminate(lcmd->right);
	}
	return (cmd);
}
