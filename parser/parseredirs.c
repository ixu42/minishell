/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseredirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 10:47:48 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/25 12:03:15 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

t_cmd	*make_redir_node(t_cmd *cmd, t_strstate *state, \
	int tok, t_aststate *ast)
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
		/*   
		cmd->flag |= state->flag; // alex will continue
		cmd = redircmd(cmd, state, O_RDONLY, 0);
		cmd->flag |= state->flag;
		*/
		if (state->flag == 0)
			cmd = redircmd(cmd, state, O_RDONLY, 0);
		else
			cmd->flag |= state->flag;
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
