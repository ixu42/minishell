#include "../minishell.h"

// parsecmd.c


t_cmd*	parsepipe(char **ps, char *es)
{
	t_cmd *cmd;
	int		tok;

	cmd = parseexec(ps, es);
	tok = peek(ps, es, "|");
	if(tok && (*ps)[1] != '|')
	{
		tok = gettoken(ps, es, 0, 0);
		if (tok == PIPE_TOK)
			cmd = pipecmd(cmd, parsepipe(ps, es));
		else
		{
			ft_dprintf(2, "From parsepipe:\n", PMT, ERR_SYNTAX_UNEXP);
			ft_dprintf(2, "%s %s\n", PMT, ERR_SYNTAX_UNEXP);
		}
	}
	return (cmd);
}

t_cmd	*parseline(char **ps, char *es)
{
	t_cmd *cmd_a;
	t_cmd *cmd_b;
	int		tok;
	int		cond;

	cmd_a = parsepipe(ps, es);
	cond = peek(ps, es, "&|");
	while (cond && (*ps)[0] == (*ps)[1])
	//while (1)
	{
		tok = gettoken(ps, es, 0, 0);
//		printf("tok = %d\n", tok);
	//	printf("ps=%s\n", *ps);
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
//	printf("str=%s\n", *ps);
	return (cmd_a);
}

t_cmd	*parseblock(char **ps, char *es)
{
	t_cmd *cmd;

	if (!peek(ps, es, "("))
		panic_test("parseblock");
	gettoken(ps, es, 0, 0);
	cmd = parseline(ps, es);
	if (!peek(ps, es, ")"))
		panic_test("syntax - missing )");
	gettoken(ps, es, 0, 0);
	cmd = parseredirs(cmd, ps, es);
	return (cmd);
}

t_cmd	*parsecmd(char *s)
{
	char		*es;
	t_cmd	*cmd;
	
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
//	printf("leftour s=%s\n", s);
	peek(&s, es, "");
	if(s != es)
	{
		ft_putstr_fd("leftovers after parsed line", 2);
		ft_putstr_fd(s, 2);
		ft_putstr_fd("\n", 2);
		panic_test("syntax error after parsing line");
	}
	nulterminate(cmd);
	return (cmd);
}
