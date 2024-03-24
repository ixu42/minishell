/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gettoken_peek.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 10:32:27 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/24 10:41:20 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

const char	*token_type_to_str(t_token_type token)
{
	if (token == UNDEFINED_TOK)
		return ("UNDEF");
	else if (token == NEWLINE_TOK)
		return ("newline");
	else if (token == STR_TOK)
		return ("a");
	else if (token == RED_IN)
		return ("<");
	else if (token == RED_OUT)
		return (">");
	else if (token == HEREDOC)
		return ("<<");
	else if (token == RED_OUT_APP)
		return (">>");
	else if (token == PIPE_TOK)
		return ("|");
	else if (token == OR_TOK)
		return ("||");
	else if (token == AND_TOK)
		return ("&&");
	else if (token == LPAR)
		return ("(");
	else if (token == RPAR)
		return (")");
	return ("UNKNOWN");
}

void	gettoken_quotes(char **s, char *es)
{
	int		quotes[2];

	quotes[0] = 0;
	quotes[1] = 0;
	while (*s < es && ((quotes[0] || quotes[1]) || \
				(!ft_strchr(WHITESPACE, **s) && !ft_strchr(SYMBOLS, **s))))
		increase_s_quotes(s, quotes);
}

int	gettoken(char **ps, char *es, char **q, char **eq)
{
	char	*s;
	int		ret;

	s = *ps;
	while (s < es && ft_strchr(WHITESPACE, *s))
		s++;
	if (q)
		*q = s;
	ret = UNDEFINED_TOK;
	if (ft_strchr(SYMBOLS, *s) || *s == '\0')
		ret = select_token(&s);
	else
	{
		ret = STR_TOK;
		gettoken_quotes(&s, es);
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
	char	*s;

	s = *ps;
	while (s < es && ft_strchr(WHITESPACE, *s))
		s++;
	*ps = s;
	return (*s && ft_strchr(toks, *s));
}
