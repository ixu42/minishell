#include "../minishell.h"

// parsecmd.c


t_cmd*	parsepipe(char **ps, char *es)
{
	t_cmd *cmd;
	int		tok;

	cmd = parseexec(ps, es);
	if (cmd->flag)
		return (cmd);
	tok = peek(ps, es, "|");
	if (tok && (*ps)[1] != '|')
	{
		tok = gettoken(ps, es, 0, 0);
		cmd = pipecmd(cmd, parsepipe(ps, es));
		/*
		if (tok == PIPE_TOK)
			cmd = pipecmd(cmd, parsepipe(ps, es));
		//next 4 lines newer happens 
		else
		{
			ft_dprintf(2, "From parsepipe:\n", PMT, ERR_SYNTAX_UNEXP);
			ft_dprintf(2, "%s %s\n", PMT, ERR_SYNTAX_UNEXP);
		}
		*/
	}
//	printf("parsepipe: ps=%s\n", *ps);
	return (cmd);
}

t_cmd	*parseline(char **ps, char *es)
{
	t_cmd *cmd_a;
	t_cmd *cmd_b;
	int		tok;
	int		cond;

	cmd_a = parsepipe(ps, es);
	if (cmd_a->flag)
		return (cmd_a);
	cond = peek(ps, es, "&|");
	while (cond && (*ps)[0] == (*ps)[1])
	{
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
			break ; //return (cmd_a);
		cond = peek(ps, es, "&|");
	}
//	printf("parseline: ps=%s\n", *ps);
	return (cmd_a);
}

t_cmd	*parseblock(char **ps, char *es)
{
	t_cmd *cmd;

	if (!peek(ps, es, "("))
		ft_dprintf(2, "parseblock");
	gettoken(ps, es, 0, 0);
	cmd = parseline(ps, es);
	if (!peek(ps, es, ")"))
		ft_dprintf(2, "syntax - missing )");
	gettoken(ps, es, 0, 0);
	cmd = parseredirs(cmd, ps, es);
	return (cmd);
}

t_cmd	*parsecmd(char *s)
{
	char		*es;
	t_cmd	*cmd;
	int		tok;
	
	if (s == NULL)
	{
		dprintf(2, "Error: buffer cmd = NULL\n");
		return (NULL);
	}
	if (*s == '\0')
	{
		dprintf(2, "Error: buffer cmd is empty string\n");
		return (NULL);
	}

	es = s + ft_strlen(s);
	cmd = parseline(&s, es);
	if (cmd->flag || s != es)
	{
		tok = gettoken(&s, es, 0, 0);
	//	ft_dprintf(2,"%s %s '%s'\n", PMT, ERR_SYNTAX_UNEXP, token_type_to_str(tok));
		ft_dprintf(2,"%s %s ", PMT, ERR_SYNTAX_UNEXP);
		if (tok == UNDEFINED_TOK)
			ft_dprintf(2, "'%s'\n", s);
		else
			ft_dprintf(2,"'%s'\n", token_type_to_str(tok));
	}
/*	ft_dprintf(2,"Leftover %s\n", s);
	peek(&s, es, "");
	//if(s != es)
	//	ft_dprintf(2,"Leftover %s\n", s);
	ft_dprintf(2,"Leftover %s\n", s);

*/
	nulterminate(cmd);
	return (cmd);
}
