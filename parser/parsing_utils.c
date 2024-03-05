#include "../minishell.h"

// parsing_utils.c
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

int	set_token(char **pnt, int tok_size, int tok_code)
{
	*pnt +=tok_size;
	return (tok_code);
}

int	select_token(char **pnt)
{
	char	*s;
	int		ret;

	s = *pnt;
	if (s[0] == s[1] && s[0] == '>' && !ft_strchr(SYMBOLS, s[2]))
		ret = set_token(&s, 2, RED_OUT_APP);
	else if (s[0] == s[1] && s[0] == '<' && !ft_strchr(SYMBOLS, s[2]))
		ret = set_token(&s, 2, HEREDOC);
	else if (s[0] == s[1] && s[0] == '&' && !ft_strchr("&|", s[2]))
		ret = set_token(&s, 2, AND_TOK);
	else if (s[0] == s[1] && s[0] == '|' && !ft_strchr("|&", s[2]))
		ret = set_token(&s, 2, OR_TOK);
	else if (s[0] == '>' && !ft_strchr(SYMBOLS, s[1]))
		ret = set_token(&s, 1, RED_OUT);
	else if (s[0] == '<' && !ft_strchr(SYMBOLS, s[1]))
		ret = set_token(&s, 1, RED_IN);
	else if (s[0] == '|' && !ft_strchr("|&",s[1]))
		ret = set_token(&s, 1, PIPE_TOK);
	else 
		ret = UNDEFINED_TOK;
	*pnt = s;
	return (ret);
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
	ret = UNDEFINED_TOK;
	if (*s == '\0')
		;
	else if (ft_strchr(SYMBOLS, *s))
		ret = select_token(&s);
/*
	{
		if (s[0] == s[1] && s[0] == '>')
		{
			s++;
			ret = RED_OUT_APP;
		}
		else if (s[0] == s[1] && s[0] == '<')
		{
			s++;
			ret = HEREDOC;
		}
		else if (s[0] == s[1] && s[0] == '&')
		{
			s++;
			ret = AND_TOK;
		}
		else if (s[0] == s[1] && s[0] == '|')
		{
			s++;
			ret = OR_TOK;
		}
		s++;
	}
*/
	else
	{
		ret = STR_TOK;
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


int	peek(char **ps, char *es, char *toks)
{
	char *s;

	s = *ps;
	while(s < es && ft_strchr(WHITESPACE, *s))
		s++;
	*ps = s;
	return (*s && ft_strchr(toks, *s));
}

// NUL-terminate all the counted strings.
t_cmd	*nulterminate(t_cmd *cmd)
{
	int i;
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
		while (ecmd->sargv[i])
			*ecmd->eargv[i++] = 0;
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
	return (cmd);
}

void    panic_test(char *s)
{
//remove this function. use fd_dprintf instead and rise flags for errorcases.  
//  printf(2, "%s\n", s);
    ft_putstr_fd(s, 2);
    ft_putstr_fd("\n", 2);
 //   exit(1);
}
