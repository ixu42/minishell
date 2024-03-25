/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_star.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 11:14:51 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/24 11:14:56 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <dirent.h>

int	wildcard_star(t_execcmd *cmd)
{
	t_wildcard	wild;

	wild.directory = opendir(".");
	if (!wild.directory)
		return (7);
	if (init_wildcard(&wild, cmd))
		return (1);
	if (match_to_files(&wild))
		return (2);
	if (make_sorted_argv(&wild, 1))
		return (3);
	if (copy_sorted_argv(&wild))
		return (4);
	ft_free_char2d(cmd->argv);
	cmd->argv = wild.list->data;
	cmd->argc = wild.argc;
	cmd->list = wild.list;
	free_wildcard(&wild, 0, 0);
	return (0);
}

int	make_filename_with_wildcard(t_wildcard *wild, t_redircmd *cmd)
{
	wild->directory = opendir(".");
	if (!wild->directory)
		return (WILD_ERR_TERMINATE);
	if (init_wildcard_redir(wild, cmd))
		return (WILD_ERR_TERMINATE);
	if (match_to_files(wild))
		return (WILD_ERR_TERMINATE);
	if (make_sorted_argv(wild, 0))
		return (WILD_ERR_TERMINATE);
	if (copy_sorted_argv(wild))
		return (WILD_ERR_TERMINATE);
	return (0);
}

int	wildcard_star_redir(t_redircmd *cmd)
{
	t_wildcard	wild;

	if (make_filename_with_wildcard(&wild, cmd))
		return (WILD_ERR_TERMINATE);
	if (wild.pnt[0]->size > 1)
	{
		ft_dprintf(2, "%s %s %s\n", PMT, cmd->sfile, ERR_REDIR_AMBIG);
		free(cmd->file);
		free_wildcard(&wild, 1, 0);
		ft_free_char2d(wild.argv);
		wild.argv = NULL;
		return (WILD_ERR_REDIR_AMBIG);
	}
	cmd->list = wild.list;
	free(cmd->file);
	cmd->file = wild.list->data[0];
	free_wildcard(&wild, 0, 0);
	ft_free_char2d(wild.argv);
	wild.argv = NULL;
	return (0);
}
