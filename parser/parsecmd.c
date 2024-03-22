#include "../minishell.h"

// parsecmd.c

t_cmd	*parsepipe(char **ps, char *es,t_aststate *ast)
{
	t_cmd	*cmd;
	int		tok;

	cmd = parseexec(ps, es, ast);
	if (!cmd)
		return (NULL);
	if (cmd->flag)
		return (cmd);
	tok = peek(ps, es, "|");
	if (tok && (*ps)[1] != '|')
	{
		tok = gettoken(ps, es, 0, 0);
		cmd = pipecmd(cmd, parsepipe(ps, es, ast));
	}
	return (cmd);
}

int	make_listcmd(t_cmd **p_cmd_a, char **ps, char *es, t_aststate *ast)
{
	t_cmd	*cmd_a;
	t_cmd	*cmd_b;
	int		tok;

	cmd_a = *p_cmd_a;
	tok = gettoken(ps, es, 0, 0);
	if (tok == AND_TOK)
	{
		cmd_b = parsepipe(ps, es, ast);
		cmd_a = list_cmd(cmd_a, cmd_b, AND_CMD);
	}
	else if (tok == OR_TOK)
	{
		cmd_b = parsepipe(ps, es, ast);
		cmd_a = list_cmd(cmd_a, cmd_b, OR_CMD);
	}
	else
		return (1);
	*p_cmd_a = cmd_a;
	return (0);
}

t_cmd	*parseline(char **ps, char *es, t_aststate *ast)
{
	t_cmd	*cmd_a;
	int		cond;

	cmd_a = parsepipe(ps, es, ast);
	if (!cmd_a)
		return (NULL);
	if (cmd_a->flag)
		return (cmd_a);
	cond = peek(ps, es, "&|");
	while (cond && (*ps)[0] == (*ps)[1])
	{
		if (make_listcmd(&cmd_a, ps, es, ast))
			break ;
		if (!cmd_a)
			break ;
		if (cmd_a->flag)
			break ;
		cond = peek(ps, es, "&|");
	}
	return (cmd_a);
}

// redir after block -> syntax error
t_cmd	*parseblock(char **ps, char *es, t_aststate *ast)
{
	t_cmd	*cmd;

	(*ps)++;
	cmd = parseline(ps, es, ast);
	if (!cmd)
		return (NULL);
	if (cmd->flag)
		return (cmd);
//	if (peek(ps, es, ")") && cmd->flag == 0)
	if (peek(ps, es, ")"))
		(*ps)++;
	else
		cmd->flag |= SYNTAX_ERROR;
//	if (cmd->flag == 0)
	//	cmd = parseredirs(cmd, ps, es);
	return (cmd);
}

int	cmd_status(char *s, t_cmd *cmd, char *es)
{
	int		tok;
	char	*str[2];

	if (cmd->flag & SYNTAX_ERR_UNCLOSED)
		return (ERR_CODE_SYNTAX);
	if (cmd->flag & MALLOC_ERROR)
		return (panic_parser(ERR_MALLOC, ENOMEM_ERR));
	else if (cmd->flag & HEREDOC_OPEN_ERR)
		return (ENOMEM_ERR);
	else if (cmd->flag & SIGNAL_CTRL_C)
		return (SIGNAL_CTRL_C);
	else if (cmd->flag || s != es)
	{
		ft_dprintf(2, "%s%s ", PMT, ERR_SYNTAX_UNEXP);
		tok = gettoken(&s, es, &str[0], &str[1]);
		if (tok == UNDEFINED_TOK)
			ft_dprintf(2, "'%c'\n", *str[0]);
		else if (tok == STR_TOK)
		{
			*str[1] = '\0';
			ft_dprintf(2, "'%s'\n", str[0]);
		}
		else
			ft_dprintf(2, "'%s'\n", token_type_to_str(tok));
		return (ERR_CODE_SYNTAX);
	}
	return (0);
}

t_cmd	*parsecmd(char *s, int *status)
{
	char	*es;
	t_cmd	*cmd;	
	int		ret;
	t_aststate	*ast;

	ret = 0;
	es = s + ft_strlen(s);

	ast = make_aststate(s, es);
	if (!ast)
		return (NULL);
	cmd = parseline(&s, es, ast);
	free(ast);
	if (!cmd)
		return (NULL);
	ret = cmd_status(s, cmd, es);
	nulterminate(cmd);
	if (status)
		*status = ret;
	return (cmd);
}

int	make_ast(t_cmd **p_cmd, char *s)
{
	int		status;

	*p_cmd = parsecmd(s, &status);
	if (!*p_cmd)
		status = ENOMEM_ERR;
	return (status);
}
