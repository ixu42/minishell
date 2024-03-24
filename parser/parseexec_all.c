#include "../minishell.h"

//parseexec.c
//t_strcmd	*parsestr(t_strstate *state);

// parseredirs.c
t_cmd	*combine_redirs(t_cmd *head, t_cmd *extra, t_cmd *cmd)
{
	t_cmd		*tail;
	t_redircmd	*last;

	if (!head)
		return (NULL);
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
		return (extra); 
	return (head);
}

void	make_redir_str(t_cmd *cmd, t_strstate *state)
{
	t_strstate	*state_str;
	t_redircmd	*rcmd;

//	if (cmd->type != REDIR || cmd->flag)
//		return ;
	rcmd = (t_redircmd *)cmd;
	state_str = make_strstate(state->beg, state->end);
	if (!state_str)
	{
		rcmd->flag |= MALLOC_ERROR;
		return ;
	}
	rcmd->str = parsestr(state_str);
	free(state_str);
	if (!rcmd->str)
	{
		rcmd->flag |= MALLOC_ERROR;
		return ;
	}
	rcmd->flag |= rcmd->str->flag;
	return ;
}

t_cmd	*make_redir_node(t_cmd *cmd, t_strstate *state, int tok, t_aststate *ast)
{
	if (cmd->flag)
		return (cmd);
	else if (tok == RED_IN)
		cmd = redircmd(cmd, state, O_RDONLY, 0);
	else if (tok == RED_OUT)
		cmd = redircmd(cmd, state, O_WRONLY | O_CREAT | O_TRUNC, 1);
	else if (tok == RED_OUT_APP)
		cmd = redircmd(cmd, state, O_WRONLY | O_CREAT | O_APPEND, 1);
	else if (tok == HEREDOC)
	{
		*state->end = '\0';
		get_input_heredoc(state, ast, state->beg);
		ast->heredoc++;
		if (state->flag == 0)
			cmd = redircmd(cmd, state, O_RDONLY, 0);
		else
			cmd->flag = state->flag;
		//cmd = redircmd(cmd, state, -1, 0);
	//	ft_dprintf(2, "i=%d, eof=->%s<-, heredoc=->%s<-\n", ast->heredoc, state->beg, state->heredoc);
	}
	return (cmd);
}

t_cmd	*parse_one_redir(t_cmd *cmd, t_strstate *state, t_aststate *ast)
{
	int		tok;

	if (peek(&(state->pos), state->finish, "<>"))
	{
		tok = gettoken(&(state->pos), state->finish, 0, 0);
		if (tok == UNDEFINED_TOK || tok == NEWLINE_TOK)
		{
			cmd->flag |= SYNTAX_ERR_UNDEFTOK | SYNTAX_ERR_UNEXPTOK;
			state->flag |= cmd->flag;
			return (cmd);
		}
		if (STR_TOK != gettoken(&(state->pos), state->finish, &(state->beg), \
			&(state->end)))
		{
			cmd->flag |= SYNTAX_ERR_UNEXPTOK;
			state->pos = state->beg;
		}
		cmd = make_redir_node(cmd, state, tok, ast);
	}
	if (cmd && cmd->type == REDIR && cmd->flag == 0)
		make_redir_str(cmd, state);
	return (cmd);
}

t_cmd	*parseredirs(t_cmd *cmd, char **ps, char *es, t_aststate *ast)
{
	t_cmd		*node;
	t_redircmd	*rcmd;
	t_strstate	*state;

	state = make_strstate(*ps, es);
	if (!state)
	{
		cmd->flag |= MALLOC_ERROR;
		return (cmd);
	}
	node = parse_one_redir(cmd, state, ast);
	*ps = state->pos;
	free(state);
	if (node->type != REDIR)
		return (node);
	rcmd = (t_redircmd *)node;
	if (peek(ps, es, "<>") && node->flag == 0)
		rcmd->cmd = parseredirs(cmd, ps, es, ast);
	rcmd->flag |= rcmd->cmd->flag;
	return (node);
}


// parse_word_singl_var.c
t_strcmd	*parse_word(t_strstate *state)
{
	char		*s;
	t_strcmd	*node;

	s = state->pos;
	state->beg = s;
	while (!ft_strchr("*$\'\"", *s) && s < state->finish)
		s++;
	state->end = s;
	state->pos = s;
	/*
	if (state->end == state->beg)
	{
		ft_dprintf(2, "Error: word of zerosize.\n");
		state->flag |= SYNTAX_ERROR;
	}
	*/
	node = strcmd(STR_NODE, state->beg, state->end);
	if (node == NULL)
		state->flag |= MALLOC_ERROR;
	else
		node->flag |= state->flag;
	return (node);
}

t_strcmd	*parse_str_till(t_strstate *state, char *stop_toks)
{
	char		*s;
	t_strcmd	*node;

	s = state->pos;
	state->beg = s;
	while (!ft_strchr(stop_toks, *s) && s < state->finish)
		s++;
	state->end = s;
	state->pos = s;
	/*
	if (state->end == state->beg)
	{
		ft_dprintf(2, "Error: str of zerosize.\n");
		state->flag |= SYNTAX_ERROR;
	}
	*/
	node = strcmd(STR_NODE, state->beg, state->end);
	if (node == NULL)
		state->flag |= MALLOC_ERROR;
	else
		node->flag |= state->flag;
	return (node);
}

t_strcmd	*parse_single(t_strstate *state)
{
	char		*s;
	t_strcmd	*node;

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
		ft_dprintf(2, "%ssyntax error: unclosed single quote \'\n", PMT);
		state->flag |= SYNTAX_ERR_UNCLOSED;
	}
	state->pos = s;
	node = strcmd(STR_NODE, state->beg, state->end);
	if (node == NULL)
		state->flag |= MALLOC_ERROR;
	else
		node->flag |= state->flag;
	return (node);
}

void	make_var_node(t_strcmd **p_node, t_strstate *state)
{
	t_strcmd	*node;

	node = strcmd(STR_NODE_VAR, state->beg, state->end);
	if (node)
	{
		node->flag |= state->flag;
		if (state->d_quotes)
			node->type = STR_NODE_VAR_P;
	}
	else
		state->flag |= MALLOC_ERROR;
	*p_node = node;
}

t_strcmd	*parse_variable(t_strstate *state)
{
	char		*s;
	t_strcmd	*node;

	s = state->pos + 1;
	state->beg = s;
	if (!ft_isdigit(*s))
	{
		while ((ft_isalnum(*s) || *s == '_') && s < state->finish)
			s++;
	}
	state->end = s;
	state->pos = s;
	if (state->end == state->beg)
	{
		state->beg--;
		node = strcmd(STR_NODE, state->beg, state->end);
		return (node);
	}
	make_var_node(&node, state);
	return (node);
}


//parsestr.c
t_strcmd	*parse_double_elem(t_strstate *state)
{
	t_strcmd	*node;

	if (*(state->pos) == '$' && *(state->pos + 1) == '?')
	{
		node = strcmd(STR_EXIT_CODE, state->pos + 1, state->pos + 2);
		state->pos += 2;
	}
	else if (*(state->pos) == '$')
		node = parse_variable(state);
	else
		node = parse_str_till(state, "$\"");
	return (node);
}

t_strcmd	*parse_double(t_strstate *state)
{
	t_strcmd	*node;

	node = parse_double_elem(state);
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
		ft_dprintf(2, "%ssyntax error: unclosed double quote \"\n", PMT);
		state->flag |= SYNTAX_ERR_UNCLOSED;
	}
	node->flag |= state->flag;
	return (node);
}

t_strcmd	*parse_element(t_strstate *state)
{
	t_strcmd	*node;

	if (state->d_quotes == 1)
		node = parse_double(state);
	else if (!ft_strchr("*$\'\"", *(state->pos)))
		node = parse_word(state);
	else if (*(state->pos) == '\'')
		node = parse_single(state);
	else if (*(state->pos) == '$' && *(state->pos + 1) == '?')
	{
		node = strcmd(STR_EXIT_CODE, state->pos + 1, state->pos + 2);
		state->pos += 2;
	}
	else if (*(state->pos) == '$')
		node = parse_variable(state);
	else if (*(state->pos) == '*')
	{
		node = strcmd(STR_STAR, state->pos, state->pos + 1);
		state->pos[0] = ASCII_WILD;
		state->pos++;
	}
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
	node = parse_element(state);
	if (!node)
		return (NULL);
	if (state->pos < state->finish && node->flag == 0)
	{
		new = parsestr(state);
		node->next = new;
		if (new == NULL)
			node->flag |= MALLOC_ERROR;
		else
			node->flag |= new->flag;
	}
	return (node);
}
