#include "../minishell.h"

int	free_str(t_strcmd *cmd)
{
	if (cmd->next)
	{
		free_str(cmd->next);
		cmd->next = NULL;
	}
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
