#include "../minishell.h"

// parsecmd.c

t_cmd*	parsepipe(char **ps, char *es)
{
	t_cmd *cmd;
	int		tok;


	cmd = parseexec(ps, es);
	if (!cmd)
		return (NULL);
	if (cmd->flag)
		return (cmd);
	tok = peek(ps, es, "|");
	if (tok && (*ps)[1] != '|')
	{
		tok = gettoken(ps, es, 0, 0);
		cmd = pipecmd(cmd, parsepipe(ps, es));
	}
	return (cmd);
}

int	make_listcmd(t_cmd **p_cmd_a, char **ps, char *es)
{
	t_cmd	*cmd_a;
	t_cmd	*cmd_b;
	int		tok;

	cmd_a = *p_cmd_a;
	tok = gettoken(ps, es, 0, 0);
	if (tok == AND_TOK)
	{
		cmd_b = parsepipe(ps, es);
		cmd_a = list_cmd(cmd_a, cmd_b, AND_CMD);
	}
	else if (tok == OR_TOK)
	{
		cmd_b = parsepipe(ps, es);
		cmd_a = list_cmd(cmd_a, cmd_b, OR_CMD);
	}
	else
		return (1);
	*p_cmd_a = cmd_a;
	return (0);
}

t_cmd	*parseline(char **ps, char *es)
{
	t_cmd *cmd_a;
	int		cond;

	cmd_a = parsepipe(ps, es);
	if (!cmd_a)
		return (NULL);
	if (cmd_a->flag)
		return (cmd_a);
	cond = peek(ps, es, "&|");
	while (cond && (*ps)[0] == (*ps)[1])
	{
		if (make_listcmd(&cmd_a, ps, es))
			break ;
		if (!cmd_a)
			break ;
		if (cmd_a->flag)
			break ;
		cond = peek(ps, es, "&|");
	}
	return (cmd_a);
}

t_cmd	*parseblock(char **ps, char *es)
{
	t_cmd *cmd;

	(*ps)++;
	cmd = parseline(ps, es);
	if (peek(ps, es, ")") && cmd->flag == 0)
		(*ps)++;
	else
		cmd->flag |= SYNTAX_ERROR;
//	gettoken(ps, es, 0, 0);
//	ft_dprintf(2, "parseblock: *ps=%s, cmd->flag=%d\n", *ps, cmd->flag);
//	if (cmd->flag == 0)
	//	cmd = parseredirs(cmd, ps, es);
	return (cmd);
}

t_cmd	*parsecmd(char *s)
{
	char	*es;
	char	*str[2];
	t_cmd	*cmd;
	int		tok;
	
/*	if (s == NULL)
	{
		dprintf(2, "Error: buffer cmd = NULL\n");
		return (NULL);
	}
	if (*s == '\0')
	{
		dprintf(2, "Error: buffer cmd is empty string\n");
		return (NULL);
	}
*/
	es = s + ft_strlen(s);
	cmd = parseline(&s, es);
//	printf("flag? = %d\n", cmd->flag & SYNTAX_ERR_UNCLOSED);
	if (cmd->flag & SYNTAX_ERR_UNCLOSED)
		;
	else if (cmd->flag || s != es)
	{
		ft_dprintf(2,"%s %s ", PMT, ERR_SYNTAX_UNEXP);
		tok = gettoken(&s, es, &str[0], &str[1]);
		if (tok == UNDEFINED_TOK)
			ft_dprintf(2, "'%c'\n", *str[0]);
		else if (tok == STR_TOK)
		{
			*str[1] = '\0';
			ft_dprintf(2, "'%s'\n", str[0]);
		}
		else
			ft_dprintf(2,"'%s'\n", token_type_to_str(tok));
	}
	nulterminate(cmd);
	return (cmd);
}
