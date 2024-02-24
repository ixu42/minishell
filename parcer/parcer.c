#include "../minishell.h"

#define WHITESPACE	" \t\r\n\v"
//#define SYMBOLS	"<>|&()\"\'"
#define SYMBOLS	"<>|&;()\\"
//#define MAXARGS 3

int	fork1_test(void);	// Fork but panics on failure.
void	panic_test(char*);
t_cmd	*parsecmd(char*);

// test version of runcmd for testing AST
void	runcmd_test(t_cmd *cmd)
{
	int p[2];
	t_backcmd *bcmd;
	t_execcmd *ecmd;
	t_listcmd *lcmd;
	t_pipecmd *pcmd;
	t_redircmd *rcmd;

	if(cmd == NULL)
	{
		printf("NULL Node\n");
		exit(1);
	}
	else if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd*)cmd;
		if(ecmd->argv[0] == 0)
			exit (1);
		printf("argv=%s, %s, %s, %s\n", ecmd->argv[0], ecmd->argv[1], ecmd->argv[2], ecmd->argv[3]);
	//		exec(ecmd->argv[0], ecmd->argv);
	 // printf(2, "exec %s failed\n", ecmd->argv[0]);
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd*)cmd;
		printf("close(fd=%d);\n",rcmd->fd);
		printf("open(%s, mode=%d);\n", rcmd->file, rcmd->mode);
		/*
		if (open(rcmd->file, rcmd->mode) < 0)
		{
			printf(2, "open %s failed\n", rcmd->file);
			exit (1);
		}
		*/
		runcmd_test(rcmd->cmd);
	}
	else if (cmd->type == LIST)
	{
		lcmd = (t_listcmd*)cmd;
		if(fork1_test() == 0)
			runcmd_test(lcmd->left);
		wait(NULL);
		runcmd_test(lcmd->right);
	}
	else if (cmd->type == AND_CMD)
	{
		lcmd = (t_listcmd*)cmd;
		if(fork1_test() == 0)
			runcmd_test(lcmd->left);
		wait(NULL);
		printf("&&\n");
		runcmd_test(lcmd->right);
	}
	else if (cmd->type == OR_CMD)
	{
		lcmd = (t_listcmd*)cmd;
		if(fork1_test() == 0)
			runcmd_test(lcmd->left);
		wait(NULL);
		printf("||\n");
		runcmd_test(lcmd->right);
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd*)cmd;
	//	if (pipe(p) < 0)
		//	panic_test("pipe");
		printf("make pipe\n");
		if (fork1_test() == 0){
//			close(1);
	//		dup(p[1]);
		//	close(p[0]);
		//	close(p[1]);
			runcmd_test(pcmd->left);
		}
		wait(NULL);
		if (fork1_test() == 0){
//			close(0);
	//		dup(p[0]);
		//	close(p[0]);
			//close(p[1]);
			runcmd_test(pcmd->right);
		}
//		close(p[0]);
	//	close(p[1]);
		wait(NULL);
	}
	else if (cmd->type == BACK)
	{
		bcmd = (t_backcmd*)cmd;
		if(fork1_test() == 0)
			runcmd_test(bcmd->cmd);
		wait(NULL);
	}
	else
		panic_test("runcmd");
	exit (0);
}

void	panic_test(char *s)
{
//	printf(2, "%s\n", s);
	ft_putstr_fd(s, 2);
	ft_putstr_fd("\n", 2);
	exit(1);
}

int fork1_test(void)
{
	int pid;

	pid = fork();
	if(pid == -1)
		panic_test("fork");
	return pid;
}

// Constructors

t_cmd	*execcmd(void)
{
	t_execcmd *cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = EXEC;
	return ((t_cmd*)cmd);
}

t_cmd	*argcmd(void)
{
	t_argcmd *cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = ARG_NODE;
	return ((t_cmd*)cmd);
}

t_cmd	*strcmd(int type)
{
	t_strcmd *cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = type;
	return ((t_cmd*)cmd);
}

t_cmd* redircmd(t_cmd *subcmd, char *file, char *efile, int mode, int fd)
{
	t_redircmd *cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = REDIR;
	cmd->cmd = subcmd;
	cmd->file = file;
	cmd->efile = efile;
	cmd->mode = mode;
	cmd->fd = fd;
	return ((t_cmd*)cmd);
}

t_cmd* pipecmd(t_cmd *left, t_cmd *right)
{
	t_pipecmd *cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return (t_cmd*)cmd;
}

t_cmd	*listcmd(t_cmd *left, t_cmd *right)
{
	t_listcmd *cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = LIST;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd*)cmd);
}

t_cmd	*list_cmd(t_cmd *left, t_cmd *right, int type)
{
	t_listcmd *cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = type;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd*)cmd);
}

t_cmd	*backcmd(t_cmd *subcmd)
{
	t_backcmd *cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = BACK;
	cmd->cmd = subcmd;
	return ((t_cmd*)cmd);
}

// Parsing

void increase_s_quotes(char **pnt_s, int *p_quotes)
{
//	printf("%c\n",**pnt_s);
//	printf("q0=%d\n",p_quotes[0]);
//	printf("q1=%d\n",p_quotes[1]);
	if (**pnt_s == '\"' && p_quotes[1] == 0)
		p_quotes[0] = (p_quotes[0] + 1) % 2;
	if (**pnt_s == '\'' && p_quotes[0] == 0)
		p_quotes[1] = (p_quotes[1] + 1) % 2;
	(*pnt_s)++;
}


int	gettoken(char **ps, char *es, char **q, char **eq)
{
	char *s;
	int ret;
	int quotes[2]; //used as counters

	quotes[0] = 0;
	quotes[1] = 0;
	s = *ps;
	while (s < es && ft_strchr(WHITESPACE, *s))
		s++;
	if(q)
		*q = s;
	ret = *s;
	if (*s == 0)
		;
	else if (ft_strchr(SYMBOLS, *s))
	{
		if (s[0] == s[1] && s[0] == '>')
		{
			s++;
			ret = '+';
		}
		if (s[0] == s[1] && s[0] == '&')
		{
			s++;
			ret = AND_TOK;
		}
		if (s[0] == s[1] && s[0] == '|')
		{
			s++;
			ret = OR_TOK;
		}
		s++;
	}
	else
	{
		ret = 'a';
			while (s < es && ((!ft_strchr(WHITESPACE, *s) && !ft_strchr(SYMBOLS, *s)) \
				|| (quotes[0] || quotes[1])))
				increase_s_quotes(&s, quotes);
	}
	if (eq)
		*eq = s;
	while (s < es && ft_strchr(WHITESPACE, *s))
		s++;
	*ps = s;
	return (ret);
}


/*

int	gettoken(char **ps, char *es, char **q, char **eq)
{
	char *s;
	int ret;

	s = *ps;
	while (s < es && ft_strchr(WHITESPACE, *s))
		s++;
	
	if(q)
		*q = s;
	ret = *s;
	if (*s == 0)
		;
	else if (ft_strchr(SYMBOLS, *s))
	{
		if (s[0] == s[1] && s[0] == '>')
		{
			s++;
			ret = '+';
		}
		s++;
	}
	else
	{
		ret = 'a';
		while(s < es && !ft_strchr(WHITESPACE, *s) && !ft_strchr(SYMBOLS, *s))
			s++;
	}
	if (eq)
		*eq = s;
	while (s < es && ft_strchr(WHITESPACE, *s))
		s++;
	*ps = s;
	return (ret);
}

*/

int	peek(char **ps, char *es, char *toks)
{
	char *s;

	s = *ps;
	while(s < es && ft_strchr(WHITESPACE, *s))
		s++;
	*ps = s;
	return (*s && ft_strchr(toks, *s));
}

t_cmd	*parseline(char**, char*);
t_cmd	*parsepipe(char**, char*);
t_cmd	*parseexec(char**, char*);
t_cmd	*nulterminate(t_cmd*);

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
		ft_putstr_fd("leftovers after paesed line", 2);
		ft_putstr_fd(s, 2);
		ft_putstr_fd("\n", 2);
		panic_test("syntax error after parsing line");
	}
	nulterminate(cmd);
	return (cmd);
}

/*
t_cmd	*parseline(char **ps, char *es)
{
	t_cmd *cmd;

	cmd = parsepipe(ps, es);
	while (peek(ps, es, "&"))
	{
		gettoken(ps, es, 0, 0);
		cmd = backcmd(cmd);
	}
	if(peek(ps, es, ";"))
	{
		gettoken(ps, es, 0, 0);
		cmd = listcmd(cmd, parseline(ps, es));
	}
	return (cmd);
}
*/

//t_cmd	*parseline_and_or(char **ps, char *es)
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

t_cmd*	parsepipe(char **ps, char *es)
{
	t_cmd *cmd;
	int		tok;

	cmd = parseexec(ps, es);
	if(peek(ps, es, "|") && (*ps)[1] != '|')
	{
		tok = gettoken(ps, es, 0, 0);
		if (tok == '|')
			cmd = pipecmd(cmd, parsepipe(ps, es));
	}
	return (cmd);
}

/*
t_cmd*	parseredirs_old(t_cmd *cmd, char **ps, char *es)
{
	int tok;
	char *q, *eq;

	while (peek(ps, es, "<>"))
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
*/

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


void	attach_to_node(t_cmd *node, t_cmd *new)
{
	t_redircmd *rnode;
	
	rnode = (t_redircmd *)node;
	rnode->cmd = new;
}
/*
t_cmd *do_redirs(t_cmd *last_node, t_cmd *cmd, char **ps, char *es)
{
	t_cmd *temp;
	t_cmd *head;

	head = NULL;
	temp = parseredirs((t_cmd *)cmd, ps, es);
	if (temp != (t_cmd *)cmd)
		{
			// attaching temp to last_node
			if (last_node != (t_cmd *)cmd)
				attach_to_node(last_node, temp);
			else
				head = temp;
			last_node = temp;
		}

}
*/

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
	cmd->args = argcmd();
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

	// printf("eq=%s\n", eq);
	// make node t_arg *node
	// pass q and eq to arg_node maker
		
		extra = parseredirs((t_cmd *)cmd,  ps, es);
		head = combine_redirs(head, extra, (t_cmd *)cmd);
	/*	if (temp != (t_cmd *)cmd)
		{
			// attaching temp to last_node
			if (last_node != (t_cmd *)cmd)
				attach_to_node(last_node, temp);
			else
				ret = temp;
			last_node = temp;
		}
	*/
		// end REDIR
		
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

// NUL-terminate all the counted strings.
t_cmd	*nulterminate(t_cmd *cmd)
{
	int i;
	t_backcmd *bcmd;
	t_execcmd *ecmd;
	t_listcmd *lcmd;
	t_pipecmd *pcmd;
	t_redircmd *rcmd;

	if(cmd == 0)
		return (NULL);
	else if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd*)cmd;
		i = 0;
		while (ecmd->argv[i])
			*ecmd->eargv[i++] = 0;
		//for(i=0; ecmd->argv[i]; i++)
			//*ecmd->eargv[i] = 0;
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd*)cmd;
		nulterminate(rcmd->cmd);
		*rcmd->efile = 0;
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd*)cmd;
		nulterminate(pcmd->left);
		nulterminate(pcmd->right);
	}
	else if (cmd->type == LIST || cmd->type == AND_CMD || cmd->type == OR_CMD)
	{
		lcmd = (t_listcmd*)cmd;
		nulterminate(lcmd->left);
		nulterminate(lcmd->right);
	}
	else if (cmd->type == BACK)
	{
		bcmd = (t_backcmd*)cmd;
		nulterminate(bcmd->cmd);
	}
	return (cmd);
}
