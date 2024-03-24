/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseexec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 10:33:43 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/24 18:25:02 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	extend_arg_node(t_argcmd **arg, char *q, char *eq)
{
	t_strcmd	*str_node;
	t_argcmd	*new_node;
	t_strstate	*state;

	state = make_strstate(q, eq);
	if (!state)
		return (1);
	str_node = parsestr(state);
	free(state);
	if (!str_node)
		return (1);
	new_node = argcmd(str_node, NULL, q, eq);
	if (!new_node)
	{
		if (*arg)
			(*arg)->flag |= MALLOC_ERROR;
		return (1);
	}
	if (*arg)
		(*arg)->right = new_node;
	*arg = new_node;
	return (0);
}

void	set_execcmd_sargv(t_execcmd *cmd, char **tok_str)
{
	if (cmd->argc < MAXARGS - 1)
	{
		cmd->sargv[cmd->argc] = tok_str[0];
		cmd->eargv[cmd->argc] = tok_str[1];
	}
}

int	add_redirections(t_cmd **head, t_execcmd *cmd, char **ps, t_aststate *ast)
{
	t_cmd	*new_redir;

	new_redir = parseredirs((t_cmd *)cmd, ps, ast->es, ast);
	if (!new_redir)
	{
		(*head)->flag |= MALLOC_ERROR;
		return (1);
	}
	*head = combine_redirs(*head, new_redir, (t_cmd *)cmd);
	if (new_redir->flag)
	{
		(*head)->flag |= new_redir->flag;
		return (1);
	}
	return (0);
}

int	exec_redir_loop(t_cmd **head, t_execcmd *cmd, char **ps, t_aststate *ast)
{
	char		*tok_str[2];
	int			tok;
	t_argcmd	*new_arg;

	new_arg = NULL;
	while (!peek(ps, ast->es, "|)&"))
	{
		if (*ps == ast->es)
			break ;
		//if (STR_TOK != gettoken(ps, ast->es, tok_str, tok_str + 1))
		tok = gettoken(ps, ast->es, tok_str, tok_str + 1);
		if (tok != STR_TOK)
		{
			if (*head)
				(*head)->flag |= SYNTAX_ERR_UNDEFTOK;
			return (1);
		}
		extend_arg_node(&new_arg, tok_str[0], tok_str[1]);
		if (cmd->argc == 0)
			cmd->args = new_arg;
		cmd->flag |= new_arg->flag;
		set_execcmd_sargv(cmd, tok_str);
		cmd->argc++;
		if (add_redirections(head, cmd, ps, ast))
			return (1);
	}
	return (0);
}

t_cmd	*parseexec(char **ps, char *es, t_aststate *ast)
{
	t_execcmd	*cmd;
	t_cmd		*head;

	if (peek(ps, es, "("))
		return (parseblock(ps, es, ast));
	head = execcmd();
	if (!head)
		return (NULL);
	cmd = (t_execcmd *)head;
	head = parseredirs((t_cmd *)cmd, ps, es, ast);
	if (cmd->flag || head->flag)
		return (head);
	ast->es = es;
	exec_redir_loop(&head, cmd, ps, ast);
	if (cmd->args)
		cmd->flag |= cmd->args->flag;
	if (head == (t_cmd *)cmd && cmd->argc == 0)
		cmd->flag |= SYNTAX_ERROR;
	return (head);
}
