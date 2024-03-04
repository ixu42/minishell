#include "../minishell.h"

//parseexec.c
t_strcmd	*parsestr(t_strstate *state);

// parseredirs.c
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

void	make_redir_str(t_cmd *cmd, t_strstate *state)
{
	t_strstate	*state_str;
	t_redircmd	*rcmd;


	if (cmd->type != REDIR)
		return ;
	rcmd = (t_redircmd *)cmd;
	state_str = make_strstate(state->beg, state->end);
	if (!state_str)
	{	
		rcmd->flag = MALLOC_ERROR;
		return ;
	}
	rcmd->str = parsestr(state_str);
	if (rcmd->str == NULL)
	{
		rcmd->flag = rcmd->str->flag;
		return ;
	}
	return ;
}

t_cmd*	parse_one_redir(t_cmd *cmd, t_strstate *state)
{
	int		tok;

	if (!state)
		return (NULL);
	if (peek(&(state->pos), state->finish, "<>"))
	{
		tok = gettoken(&(state->pos), state->finish, 0, 0);
		if (gettoken(&(state->pos), state->finish, &(state->beg), &(state->end)) != STR_TOK)
			ft_dprintf(2,"Syntax error: missing name for redirection\n");
//		printf("parse_one_redir: ps=%s\n", state->pos);
		if (tok == '<')
			cmd = redircmd(cmd, state, O_RDONLY, 0);
		else if (tok == '>')
			cmd = redircmd(cmd, state, O_WRONLY | O_CREAT | O_TRUNC, 1);
		else if (tok == RED_OUT_APP)
			cmd = redircmd(cmd, state, O_WRONLY| O_CREAT | O_APPEND, 1);
		else if (tok == HEREDOC)
			cmd = redircmd(cmd, state, -1, 0);
	}
	if (!cmd)
		return (NULL);
	make_redir_str(cmd, state);
	return (cmd);
}

t_cmd *parseredirs(t_cmd *cmd, char **ps, char *es)
{
	t_cmd *node;
	t_redircmd *rcmd;
	t_strstate	*state;
	
//	ft_dprintf(2,"*ps=->%s<-\n",*ps);
	state = make_strstate(*ps, es);
	if (!state)
		return (NULL);
//	ft_dprintf(2,"*ps=->%s<-\n",*ps);
	node = parse_one_redir(cmd, state);
	*ps =state->pos;
//	ft_dprintf(2,"*ps=->%p<-\n",*ps);
//	ft_dprintf(2,"es=->%p<-\n",es);
	if (node->type != REDIR)
		return (cmd);
	rcmd = (t_redircmd *)node;
//	ft_dprintf(2," file=%s\n",rcmd->file);
//	ft_dprintf(2,"efile=%s\n",rcmd->efile);
//	free(state);
	if (peek(ps, es, "<>"))
		rcmd->cmd = parseredirs(cmd, ps, es);
	return (node);
}


// parse_word_singl_var.c
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
	/*
	if (state->end == state->beg)
	{
		ft_dprintf(2, "Error: word of zerosize.\n");
		state->flag = SYNTAX_ERROR;
	}
	*/
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
	while ((ft_isalnum(*s) || *s == '_') && s < state->finish)
		s++;
	state->end = s;
	state->pos = s;
	if (state->end == state->beg)
	{
		ft_dprintf(2, "Error: no variable name provided after $.\n");
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


//parsestr.c
t_strcmd	*parse_double_elem(t_strstate *state)
{
	t_strcmd *node;

	if (*(state->pos) == '$' && *(state->pos + 1) == '?')
	{
		node = strcmd(STR_EXIT_CODE, state->pos + 1, state->pos + 2);
		state->pos +=2;
	}
	else if (*(state->pos) == '$')
		node = parse_variable(state);
	else
		node = parse_str_till(state, "$\"");
	return (node);
}

t_strcmd	*parse_double(t_strstate *state)
{
	t_strcmd *node;

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
		ft_dprintf(2, "Syntax error: unclosed double quote \"\n");
		state->flag |= SYNTAX_ERROR;
	}
	node->flag = state->flag;
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
		state->pos +=2;
	}
	else if (*(state->pos)== '$')
		node = parse_variable(state);
	else if (*(state->pos) == '*')
	{
		node = strcmd(STR_STAR, state->pos, state->pos + 1);
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
			node->flag = new->flag;
	}
	return (node);
}


//parseexec.c
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

void	set_execcmd_argv(t_execcmd *cmd, char **tok_str)
{
		if (cmd->argc < MAXARGS - 1)
		{
			cmd->sargv[cmd->argc] = tok_str[0];
			cmd->eargv[cmd->argc] = tok_str[1];
		}
}

int	exec_redir_loop(t_cmd **head, t_execcmd *cmd, char **ps, char *es)
{
	char	*tok_str[2];
	int		tok;
	t_cmd *new_redir;
	t_argcmd *new_arg;

	new_arg = NULL;
	while(!peek(ps, es, "|)&;"))
	{
		if (*ps == es)
			break;
		tok = gettoken(ps, es, tok_str, tok_str + 1);
		if (tok != STR_TOK)  // I am not sure if is ever happens
			printf("Liteshell: exec_redir_loop: syntax error. unexpected token %d \n", tok);
		extend_arg_node(&new_arg, tok_str[0], tok_str[1]);
		if (cmd->argc == 0)
			cmd->args = new_arg;
		set_execcmd_argv(cmd, tok_str);
		cmd->argc++;
		new_redir = parseredirs((t_cmd *)cmd,  ps, es);
		*head = combine_redirs(*head, new_redir, (t_cmd *)cmd);
	}
	return (0);
}

t_cmd*	parseexec(char **ps, char *es)
{
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
	head = parseredirs((t_cmd *)cmd, ps, es);
//	printf("head =%p\n", head);
//	printf("cmd =%p\n", cmd);
	exec_redir_loop(&head, cmd, ps, es);
	//remove next two lines later 
	if (cmd->argc == 0)
		ft_dprintf(2,"parseexec:command is not specified, but it's ok.\n");
	return (head);
}
