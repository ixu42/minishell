#include "../minishell.h"

//parseexec.c

t_cmd *combine_redirs(t_cmd *head, t_cmd *extra, t_cmd *cmd)
{
	t_cmd *tail;
	t_redircmd *last;

	//tail = (t_redircmd *)head;
	tail = head;
	while (tail->type == REDIR)
	{
		if (((t_redircmd *)tail)->cmd == cmd)
			break ;
		tail = ((t_redircmd *)tail)->cmd;
	}
	if (tail->type == REDIR)
	{
		last = (t_redircmd *)tail;
		last->cmd = extra;
	}
	else if (extra->type == REDIR)
		return(extra); 
	return (head);
}

t_cmd*	parse_one_redir(t_cmd *cmd, char **ps, char *es)
{
	int tok;
	char *q, *eq;
	t_cmd *ret;

	if (peek(ps, es, "<>"))
	{
		tok = gettoken(ps, es, 0, 0);
		if (gettoken(ps, es, &q, &eq) != 'a')
			panic_test("missing file for redirection");
		if (tok == '<')
			cmd = redircmd(cmd, q, eq, O_RDONLY, 0);
		else if (tok == '>')
			cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_TRUNC, 1);
		else if (tok == '+')
			cmd = redircmd(cmd, q, eq, O_WRONLY| O_CREAT | O_APPEND, 1);
		// else toc == "-"
		// do heredoc
	}
	return (cmd);
}

t_cmd *parseredirs(t_cmd *cmd, char **ps, char *es)
{
	t_cmd *node;
	t_redircmd *rcmd;

	node = parse_one_redir(cmd, ps, es);
	if (node->type != REDIR)
		return (cmd);
	rcmd = (t_redircmd *)node;
	if (peek(ps, es, "<>"))
		rcmd->cmd = parseredirs(cmd, ps, es);
	return (node);
}

t_cmd*	parseexec(char **ps, char *es)
{
	char	*q;
	char	*eq;

	int		tok;
	int		argc;
	t_execcmd *cmd;
	t_cmd *head;
	t_cmd *extra;

	//redir
	t_cmd *last_node;
	t_cmd *temp;

	if (peek(ps, es, "("))
		return (parseblock(ps, es));
	head = execcmd();
	cmd = (t_execcmd*)head;
	argc = 0;

	// REDIRs
	head = parseredirs((t_cmd *)cmd, ps, es);
	while(!peek(ps, es, "|)&;"))
	{
		tok = gettoken(ps, es, &q, &eq);
		if (tok == 0)
			break;
		if (tok != 'a')
		{
			printf("Liteshell: -syntax error, leftover around %s \n", *ps);
			panic_test("");
		}
		cmd->argv[argc] = q;
		cmd->eargv[argc] = eq;
		argc++;
		if (argc >= MAXARGS)
			panic_test("too many args");
		extra = parseredirs((t_cmd *)cmd,  ps, es);
		head = combine_redirs(head, extra, (t_cmd *)cmd);
	}
	cmd->argv[argc] = 0;
	cmd->eargv[argc] = 0;
	if (argc == 0)
	{
		dprintf(2, "check command around:...%s\n", (*ps) - 3);
		panic_test("command is not specified, see parseexec");
	}
	return (head);
}

