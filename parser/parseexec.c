#include "../minishell.h"

//parseexec.c

t_cmd *combine_redirs(t_cmd *head, t_cmd *extra, t_cmd *cmd)
{
	t_cmd *tail;
	t_redircmd *last;

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
	int		tok;
	char	*q;
	char	*eq;
//	t_cmd	*ret;
/*	t_strstate	*state;

	state = make_strstate(q, eq); 
	if (!state)
		return (1);
	str_node = parsestr(state);
*/
	if (peek(ps, es, "<>"))
	{
		tok = gettoken(ps, es, 0, 0);
		if (gettoken(ps, es, &q, &eq) != STR_TOK)
			ft_dprintf(2,"Syntax error: missing name for redirection\n");
		if (tok == '<')
			cmd = redircmd(cmd, q, eq, O_RDONLY, 0);
		else if (tok == '>')
			cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_TRUNC, 1);
		else if (tok == RED_OUT_APP)
			cmd = redircmd(cmd, q, eq, O_WRONLY| O_CREAT | O_APPEND, 1);
		else if (tok == HEREDOC)
			cmd = redircmd(cmd, q, eq, -1, 0);
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
	ft_dprintf(2,"file=%s\n",rcmd->file);
	if (peek(ps, es, "<>"))
		rcmd->cmd = parseredirs(cmd, ps, es);
//	else
//		rcmd->cmd = cmd;
	return (node);
}


// parsing string
t_strcmd	*parse_word(t_strstate *state)
{
	char	*s;
	t_strcmd *node;

	s = state->pos;
	state->beg = s;
	while (!ft_strchr("*$\'\"", *s) && s < state->finish)
		s++;
	state->end = s;
	state->pos = s;
	if (state->end == state->beg)
	{
		ft_dprintf(2, "Error: word of zerosize.\n");
		state->flag = SYNTAX_ERROR;
	}
	node = strcmd(STR_NODE, state->beg, state->end);
	if (node == NULL)
		state->flag = MALLOC_ERROR;
	else
		node->flag = state->flag;
	return (node);
}

t_strcmd	*parse_str_till(t_strstate *state, char *stop_toks)
{
	char	*s;
	t_strcmd *node;

	s = state->pos;
	state->beg = s;
	while (!ft_strchr(stop_toks, *s) && s < state->finish)
		s++;
	state->end = s;
	state->pos = s;
	if (state->end == state->beg)
	{
		ft_dprintf(2, "Error: str of zerosize.\n");
		state->flag |= SYNTAX_ERROR;
	}
	node = strcmd(STR_NODE, state->beg, state->end);
	if (node == NULL)
		state->flag |= MALLOC_ERROR;
	else
		node->flag = state->flag;
	return (node);
}

t_strcmd	*parse_single(t_strstate *state)
{
	char	*s;
	t_strcmd *node;

	s = state->pos;
	s++;
	state->beg = s;
	while (*s != '\'' && s < state->finish)
		s++;
	state->end = s;
	if (*s == '\'')
		s++;
	else
	{
		ft_dprintf(2, "Syntax error: unclosed single quote \'\n");
		state->flag = SYNTAX_ERROR;
	}
	state->pos = s;
	node = strcmd(STR_NODE, state->beg, state->end);
	if (node == NULL)
		state->flag |= MALLOC_ERROR;
	else
		node->flag = state->flag;
	return (node);
}

t_strcmd	*parse_variable(t_strstate *state)
{
	char	*s;
	t_strcmd *node;

	s = state->pos + 1;
	state->beg = s;
	while (((47 < *s && *s < 58) || (64 < *s && *s < 91) || \
		(96 < *s && *s < 123) || *s == '_') && s < state->finish)
		s++;
	state->end = s;
	state->pos = s;
	if (state->end == state->beg)
	{
		ft_dprintf(2, "Error: variable name of zero size.\n");
		state->flag |= SYNTAX_ERROR;
	}
	node = strcmd(STR_NODE_VAR, state->beg, state->end);
	if (node)
	{
		node->flag = state->flag;
		if (state->d_quotes)
			node->type = STR_NODE_VAR_P;
	}
	else
		state->flag |= MALLOC_ERROR;
	return (node);
}

t_strcmd	*parse_double(t_strstate *state)
{
	t_strcmd *node;

	if (*(state->pos) == '$')
		node = parse_variable(state);
	else
		node = parse_str_till(state, "$\"");
	if (node == NULL)
	{
		state->flag |= MALLOC_ERROR;
		return (NULL);
	}
	if (*(state->pos) == '\"')
	{
		state->d_quotes = 0;
		state->pos++;
	}
	if (state->pos == state->finish && state->d_quotes != 0)
	{
		ft_dprintf(2, "Syntax error: unclosed double quote \"\n");
		state->flag |= SYNTAX_ERROR;
	}
	node->flag = state->flag;
	return (node);
}

t_strcmd	*parsestr(t_strstate *state)
{
	t_strcmd	*node;
	t_strcmd	*new;

	if (*(state->pos) == '\"' && state->d_quotes == 0)
	{
		state->d_quotes = 1;
		state->pos++;
	}
	if (state->d_quotes == 1)
		node = parse_double(state);
	else if (!ft_strchr("*$\'\"", *(state->pos)))
		node = parse_word(state);
	else if (*(state->pos) == '\'')
		node = parse_single(state);
	else if (*(state->pos) == '$' && *(state->pos + 1) == '?')
	{
		node = strcmd(STR_EXIT_CODE, state->pos + 1, state->pos + 2);
		state->pos +=2;
	}
	else if (*(state->pos)== '$')
		node = parse_variable(state);
	else if (*(state->pos) == '*')
	{
		node = strcmd(STR_STAR, state->pos, state->pos + 1);
		state->pos++;
	}
	if (!node)
		return (NULL);
	if (state->pos < state->finish && node->flag == 0)
	{
		new = parsestr(state);
		node->next = new;
		if (new == NULL)
			node->flag |= MALLOC_ERROR;
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
	t_strstate	*state;

	state = make_strstate(q, eq); 
	if (!state)
		return (1);
	str_node = parsestr(state);
	free(state);
	if (!str_node)
		return (1);
	last_arg = *arg;
	new_node = argcmd(str_node, NULL, q, eq);
//	new_node->right = new_node;
	if (*arg)
		(*arg)->right = new_node;
	*arg = new_node;
	return (0);
}

int	exec_redir_loop(t_cmd **head, t_execcmd *cmd, char **ps, char *es)
{
	char	*q;
	char	*eq;
	int		tok;
	t_cmd *new_redir;
	t_argcmd *new_arg;

	new_arg = NULL;
	while(!peek(ps, es, "|)&;"))
	{
		if (*ps == es)
			break;
		tok = gettoken(ps, es, &q, &eq);
		if (tok != STR_TOK)
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
		cmd->argc++;
		new_redir = parseredirs((t_cmd *)cmd,  ps, es);
		*head = combine_redirs(*head, new_redir, (t_cmd *)cmd);
	}
	if (cmd->argc <= MAXARGS - 1)
	{
		cmd->argv[cmd->argc] = 0;
		cmd->eargv[cmd->argc] = 0;
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
	printf("head =%p\n", head);
	printf("cmd =%p\n", cmd);
	exec_redir_loop(&head, cmd, ps, es);
	//remove next two lines 
	if (cmd->argc == 0)
		ft_dprintf(2,"command is not specified, but it's ok. ");
	return (head);
}

