/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructors_aux.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 10:30:45 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/24 10:41:22 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_strcmd	*strcmd(int type, char *start, char *end)
{
	t_strcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = type;
	cmd->start = start;
	cmd->end = end;
	cmd->next = NULL;
	cmd->flag = 0;
	return (cmd);
}

t_argcmd	*argcmd(t_strcmd *str, t_argcmd *args, char *start, char *end)
{
	t_argcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = ARG_NODE;
	cmd->left = str;
	if (str)
		cmd->flag = str->flag;
	cmd->right = args;
	cmd->start = start;
	cmd->end = end;
	return (cmd);
}

t_strstate	*make_strstate(char *start, char *finish)
{
	t_strstate	*state;

	state = (t_strstate *)malloc(sizeof(*state));
	if (!state)
		return (NULL);
	ft_memset(state, 0, sizeof(*state));
	state->start = start;
	state->pos = start;
	state->finish = finish;
	state->beg = NULL;
	state->end = NULL;
	state->d_quotes = 0;
	state->flag = 0;
	state->heredoc = NULL;
	return (state);
}

t_aststate	*make_aststate(char *start, char *finish)
{
	t_aststate	*state;

	state = (t_aststate *)malloc(sizeof(*state));
	if (!state)
		return (NULL);
	ft_memset(state, 0, sizeof(*state));
	state->start = start;
	state->ps = start;
	state->es = finish;
	state->flag = 0;
	state->heredoc = 0;
	return (state);
}
