/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_word_singl_var.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 10:33:30 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/25 16:45:12 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		if (node == NULL)
			state->flag |= MALLOC_ERROR;
		return (node);
	}
	make_var_node(&node, state);
	return (node);
}
