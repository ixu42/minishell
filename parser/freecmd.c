#include "../minishell.h"

int	free_str(t_strcmd *cmd)
{
	if (cmd->next)
		free_str(cmd->next);
	free(cmd);
	return (0);
}

int	free_arg(t_argcmd *cmd)
{
	if (cmd->right)
		free_arg(cmd->right);
	if (cmd->left)
		free_str(cmd->left);
	free(cmd);
	return (0);
}

int	free_exec(t_cmd *cmd)
{
	t_execcmd	*ecmd;

	ecmd = (t_execcmd *)cmd;
	if (ecmd->list)
		free_arrlist(ecmd->list);
	ecmd->list = NULL;
	if (ecmd->args)
		free_arg(ecmd->args);
	ecmd->args = NULL;
	free(cmd);
	return (0);
}

int	free_redir(t_cmd *cmd)
{
	t_redircmd	*rcmd;

	rcmd = (t_redircmd *)cmd;
	if (rcmd->list)
		free_arrlist(rcmd->list);
	rcmd->list = NULL;
	if (rcmd->str)
		free_str(rcmd->str);
	rcmd->str = NULL;
	if (rcmd->heredoc)
	{
		unlink(rcmd->heredoc);
		free(rcmd->heredoc);
		rcmd->heredoc = NULL;
	}
	if (rcmd->cmd)
		freecmd(rcmd->cmd);
	rcmd->cmd = NULL;
	free(cmd);
	return (0);
}

int	free_listcmd(t_cmd *cmd)
{
	t_listcmd	*lcmd;

	lcmd = (t_listcmd *)cmd;
//	if (lcmd->left)
	freecmd((t_cmd *)lcmd->left);
//	if (lcmd->right)
		freecmd((t_cmd *)lcmd->right);
	free(cmd);
	return (0);
}

int	free_pipe(t_cmd *cmd)
{
	t_pipecmd	*pcmd;

	pcmd = (t_pipecmd *)cmd;
//	if (pcmd->left)
	freecmd((t_cmd *)pcmd->left);
//	if (pcmd->right)
	freecmd((t_cmd *)pcmd->right);
	free(cmd);
	return (0);
}

int freecmd(t_cmd *cmd)
{
	if (!cmd)
		return (0);
	if (cmd->type == EXEC)
		return (free_exec(cmd));
	else if (cmd->type == REDIR)
		return (free_redir(cmd));
	else if (cmd->type == AND_CMD)
		return (free_listcmd(cmd));
	else if (cmd->type == OR_CMD)
		return (free_listcmd(cmd));
	else if (cmd->type == PIPE)
		return (free_pipe(cmd));
	return (0);
}
