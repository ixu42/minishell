/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 10:48:04 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/24 10:52:41 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// parsing_utils.c
void	increase_s_quotes(char **pnt_s, int *p_quotes)
{
	if (**pnt_s == '\"' && p_quotes[1] == 0)
		p_quotes[0] = (p_quotes[0] + 1) % 2;
	if (**pnt_s == '\'' && p_quotes[0] == 0)
		p_quotes[1] = (p_quotes[1] + 1) % 2;
	(*pnt_s)++;
}

int	set_token(char **pnt, int tok_size, int tok_code)
{
	if (pnt)
		*pnt += tok_size;
	return (tok_code);
}

int	select_token(char **pnt)
{
	char	*s;
	int		ret;

	s = *pnt;
	if (s[0] == s[1] && s[0] == '>')
		ret = set_token(&s, 2, RED_OUT_APP);
	else if (s[0] == s[1] && s[0] == '<')
		ret = set_token(&s, 2, HEREDOC);
	else if (s[0] == s[1] && s[0] == '&')
		ret = set_token(&s, 2, AND_TOK);
	else if (s[0] == s[1] && s[0] == '|')
		ret = set_token(&s, 2, OR_TOK);
	else if (s[0] == '>')
		ret = set_token(&s, 1, RED_OUT);
	else if (s[0] == '<')
		ret = set_token(&s, 1, RED_IN);
	else if (s[0] == '|')
		ret = set_token(&s, 1, PIPE_TOK);
	else if (s[0] == '\0')
		ret = NEWLINE_TOK;
	else
		ret = UNDEFINED_TOK;
	*pnt = s;
	return (ret);
}

t_cmd	*nulterminate(t_cmd *cmd)
{
	int			i;
	t_execcmd	*ecmd;
	t_listcmd	*lcmd;
	t_pipecmd	*pcmd;
	t_redircmd	*rcmd;

	if (cmd == 0)
		return (NULL);
	else if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		i = 0;
		while (ecmd->sargv[i])
			*ecmd->eargv[i++] = 0;
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd *)cmd;
		nulterminate(rcmd->cmd);
		*rcmd->efile = 0;
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		nulterminate(pcmd->left);
		nulterminate(pcmd->right);
	}
	else if (cmd->type == AND_CMD || cmd->type == OR_CMD)
	{
		lcmd = (t_listcmd *)cmd;
		nulterminate(lcmd->left);
		nulterminate(lcmd->right);
	}
	return (cmd);
}

int	panic_parser(char *s, int err)
{
	ft_dprintf(2, "%s %s\n", PMT, s);
	return (err);
}
