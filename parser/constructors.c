#include "../minishell.h"
// Constructors

t_cmd	*execcmd(void)
{
	t_execcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = EXEC;
	cmd->argc = 0;
	return ((t_cmd *)cmd);
}

t_strcmd	*strcmd(int type, char *start, char *end)
{
	t_strcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = type;
	cmd->start = start;
	cmd->end = end;
	cmd->next = NULL;
	cmd->flag = 0;
	return (cmd);
}

t_argcmd	*argcmd(t_strcmd *str, t_argcmd *args, char *start, char *end)
{
	t_argcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = ARG_NODE;
	cmd->left = str;
	if (str)
		cmd->flag = str->flag;
	cmd->right = args;
	cmd->start = start;
	cmd->end = end;
	return (cmd);
}


t_cmd	*redircmd(t_cmd *subcmd, t_strstate *state, int mode, int fd)
{
	t_redircmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
	{
		subcmd->flag |= MALLOC_ERROR;
		state->flag |= MALLOC_ERROR;
		return (subcmd);
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
	return ((t_cmd *)cmd);
}

t_cmd	*pipecmd(t_cmd *left, t_cmd *right)
{
	t_pipecmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	if (left)
		cmd->flag = left->flag;
	if (right)
		cmd->flag |= right->flag;
	else
		cmd->flag = MALLOC_ERROR;
	return ((t_cmd *)cmd);
}

t_cmd	*list_cmd(t_cmd *left, t_cmd *right, int type)
{
	t_listcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = type;
	cmd->left = left;
	cmd->right = right;
	if (left)
		cmd->flag = left->flag;
	if (right)
		cmd->flag |= right->flag;
	return ((t_cmd *)cmd);
}

t_strstate	*make_strstate(char *start, char *finish)
{
	t_strstate	*state;

	state = (t_strstate *)malloc(sizeof(*state));
	if (!state)
		return (NULL);
	ft_memset(state, 0, sizeof(*state));
	state->start = start;
	state->pos = start;
	state->finish = finish;
	state->beg = NULL;
	state->end = NULL;
	state->d_quotes = 0;
//	state->s_quotes = 0; //unused sofar
	state->flag = 0;
	return (state);
}
