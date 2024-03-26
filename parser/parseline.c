/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseline.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 10:33:53 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/24 10:52:13 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// parseline.c
t_cmd	*parsepipe(char **ps, char *es,	t_aststate *ast)
{
	t_cmd	*cmd;
	t_cmd	*cmd_b;
	int		tok;

	cmd = parseexec(ps, es, ast);
	if (!cmd)
		return (NULL);
	if (cmd->flag)
		return (cmd);
	tok = peek(ps, es, "|");
	if (tok && (*ps)[1] != '|')
	{
		tok = gettoken(ps, es, 0, 0);
		cmd_b = parsepipe(ps, es, ast);
		cmd = pipecmd(cmd, cmd_b);
	}
	return (cmd);
}

int	make_listcmd(t_cmd **p_cmd_a, char **ps, char *es, t_aststate *ast)
{
	t_cmd	*cmd_a;
	t_cmd	*cmd_b;
	int		tok;

	cmd_a = *p_cmd_a;
	tok = gettoken(ps, es, 0, 0);
	if (tok == AND_TOK || tok == OR_TOK)
		cmd_b = parsepipe(ps, es, ast);
	if (!cmd_b)
	{
		freecmd_null(p_cmd_a);
		return (1);
	}
	if (tok == AND_TOK)
		cmd_a = list_cmd(cmd_a, cmd_b, AND_CMD);
	else if (tok == OR_TOK)
		cmd_a = list_cmd(cmd_a, cmd_b, OR_CMD);
	else
		return (1);
	*p_cmd_a = cmd_a;
	return (0);
}

t_cmd	*parseline(char **ps, char *es, t_aststate *ast)
{
	t_cmd	*cmd_a;
	int		cond;

	cmd_a = parsepipe(ps, es, ast);
	if (!cmd_a)
		return (NULL);
	if (cmd_a->flag)
		return (cmd_a);
	cond = peek(ps, es, "&|");
	while (cond && (*ps)[0] == (*ps)[1])
	{
		if (make_listcmd(&cmd_a, ps, es, ast))
			break ;
		if (!cmd_a)
			break ;
		if (cmd_a->flag)
			break ;
		cond = peek(ps, es, "&|");
	}
	return (cmd_a);
}

// redir after block is not supported, i.e. it is syntax error
t_cmd	*parseblock(char **ps, char *es, t_aststate *ast)
{
	t_cmd	*cmd;

	(*ps)++;
	cmd = parseline(ps, es, ast);
	if (!cmd)
		return (NULL);
	if (cmd->flag)
		return (cmd);
	if (peek(ps, es, ")"))
		(*ps)++;
	else
		cmd->flag |= SYNTAX_ERROR;
	return (cmd);
}
