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
		if (gettoken(ps, es, &q, &eq) != WORD)
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

int	get_word(char **current, char *finish, char **start, char **end)
{
	char	*s;

	s = *current;
	if (start)
		*start = *current;
	while (!ft_strchr("*$\'\"", *s) && s < finish)
		s++;
	if (end)
		*end = s;
	*current = s;
	return (*s);
}

int	get_single(char **current, char *finish, char **start, char **end)
{
	char	*s;

	s = *current;
	s++;
	if (start)
		*start = *current;
	while (*s != '\'' && s < finish)
		s++;
	if (end)
		*end = s;
//	ft_dprintf(2, "s=%s\n", s);
	if (*s != '\'')
	{
		ft_dprintf(2, "Syntax error: unclosed single quote \'\n");
		return (SYNTAX_ERROR);
	}
	else
		s++;
	*current = s;
	return (*s);
}

t_strcmd	*parsestr(char *current, char *finish)
{
	t_strcmd	*node;
	t_strcmd	*new;
	int			tok;
	char		*start;
	char		*end;
	
	if (!ft_strchr("*$\'\"", *current))
	{
		tok = get_word(&current, finish, &start, &end);
		node = strcmd(STR_NODE, start, end);
	}
	else if (*current == '\'')
	{
		tok = get_single(&current, finish, &start, &end);
		node = strcmd(STR_NODE, start, end);
	}
	if (!node)
		return (NULL);
	if (tok == SYNTAX_ERROR)
	{
		node->flag = SYNTAX_ERROR;
		return (node);
	}
	if (start < end)
	{
		new = parsestr(current, finish);
		node->next = new;
		if (new == NULL)
			node->flag = MALLOC_ERROR;
		else
			node->flag = new->flag;
	}
	return (node);
}


int extend_arg_node(t_argcmd **arg, char *q, char *eq)
{
	t_strcmd *str_node;
	t_argcmd *last_arg;
	t_argcmd *new_node;

	//last_arg = *arg;

//	str_node = NULL;
	str_node = parsestr(q, eq);
	last_arg = *arg;
	new_node = argcmd(str_node, NULL, q, eq);
	new_node->right = new_node;
	(*arg)->right = new_node;
	*arg = new_node;
	return (0);
}

//int	add_argument(t_execcmd *cmd, )
int	exec_redir_loop(t_cmd *head, t_execcmd *cmd, char **ps, char *es)
{
	char	*q;
	char	*eq;
	int		tok;
	t_cmd *new_redir;
	t_argcmd *new_arg;

	while(!peek(ps, es, "|)&;"))
	{
		if (*ps == es)
			break;
		tok = gettoken(ps, es, &q, &eq);
//		if (tok == NUL_STR)
	//		break;
		if (tok != WORD)
		{
			printf("Liteshell: -syntax error, leftover around %s \n", *ps);
			panic_test("");
		}
		extend_arg_node(&new_arg, q, eq);
		if (cmd->argc == 0)
			cmd->args = new_arg;
		if (cmd->argc < MAXARGS - 1)
		{
			cmd->argv[cmd->argc] = q;
			cmd->eargv[cmd->argc] = eq;
		}
		else if (cmd->argc == MAXARGS - 1)
		{
			cmd->argv[cmd->argc] = 0;
			cmd->eargv[cmd->argc] = 0;
			//ft_dprintf(2, "number of argument more than %d", MAXARGS - 1);
		}
		cmd->argc++;
		new_redir = parseredirs((t_cmd *)cmd,  ps, es);
		head = combine_redirs(head, new_redir, (t_cmd *)cmd);
	}
	return (0);
}

t_cmd*	parseexec(char **ps, char *es)
{
	t_execcmd *cmd;
	t_cmd *head;
	
	if (peek(ps, es, "("))
		return (parseblock(ps, es));
	head = execcmd();
	if (!head)
		return (NULL);
	cmd = (t_execcmd*)head;
	head = parseredirs((t_cmd *)cmd, ps, es);
	exec_redir_loop(head, cmd, ps, es);
/*	int		tok;
	int		argc;
	t_cmd *extra;
	argc = 0;
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
	*/
	if (cmd->argc == 0)
	{
		dprintf(2, "check command around:...%s\n", (*ps) - 3);
		panic_test("command is not specified, see parseexec");
	}
	return (head);
}

