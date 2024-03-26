/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 10:32:43 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/25 15:17:40 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_syntax_err_msg(char *s, char *es)
{
	int		tok;
	char	*str[2];

	ft_dprintf(2, "%s%s ", PMT, ERR_SYNTAX_UNEXP);
	tok = gettoken(&s, es, &str[0], &str[1]);
	if (tok == UNDEFINED_TOK)
		ft_dprintf(2, "'%c'\n", *str[0]);
	else if (tok == STR_TOK)
	{
		*str[1] = '\0';
		ft_dprintf(2, "'%s'\n", str[0]);
	}
	else
		ft_dprintf(2, "'%s'\n", token_type_to_str(tok));
}

int	cmd_status(char *s, t_cmd *cmd, char *es)
{
	if (cmd->flag & SYNTAX_ERR_UNCLOSED)
		return (ERR_CODE_SYNTAX);
	if (cmd->flag & MALLOC_ERROR)
		return (panic_parser(ERR_MALLOC, ERR_CODE_TERMINATE));
	else if (cmd->flag & HEREDOC_OPEN_ERR)
		return (ERR_CODE_TERMINATE);
	else if (cmd->flag & SIGNAL_CTRL_C)
		return (SIGNAL_CTRL_C_MSG);
	else if (cmd->flag || s != es)
	{
		print_syntax_err_msg(s, es);
		return (ERR_CODE_SYNTAX);
	}
	return (0);
}

t_cmd	*parsecmd(char *s, int *status, int *num_heredoc)
{
	char		*es;
	t_cmd		*cmd;	
	int			ret;
	t_aststate	*ast;

	ret = 0;
	es = s + ft_strlen(s);
	ast = make_aststate(s, es);
	if (!ast)
		return (NULL);
	cmd = parseline(&s, es, ast);
	*num_heredoc = ast->heredoc;
	free(ast);
	if (!cmd)
		return (NULL);
	ret = cmd_status(s, cmd, es);
	nulterminate(cmd);
	if (status)
		*status = ret;
	return (cmd);
}

int	make_ast(t_cmd **p_cmd, char *s, int *num_heredoc)
{
	int		status;

	*p_cmd = parsecmd(s, &status, num_heredoc);
	if (!*p_cmd)
		return (panic_parser(ERR_MALLOC, ERR_CODE_TERMINATE));
	return (status);
}
