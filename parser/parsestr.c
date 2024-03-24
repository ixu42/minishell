#include "../minishell.h"

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
