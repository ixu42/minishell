/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_single_match.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 11:17:21 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/24 11:23:50 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	match(const char *pattern, const char *text)
{
	if (*pattern == '\0' && *text == '\0')
		return (1);
	if (*pattern == ASCII_WILD && *(pattern + 1) != '\0' && *text == '\0')
		return (0);
	if (*pattern == *text)
		return (match(pattern + 1, text + 1));
	if (*pattern == ASCII_WILD)
		return (match(pattern + 1, text) || match(pattern, text + 1));
	return (0);
}

int	init_wildcard_redir(t_wildcard *wild, t_redircmd *cmd)
{
	wild->list = create_arrlist();
	if (!wild->list)
	{
		perror(PMT);
		return (WILD_MALLOC_ERROR);
	}
	wild->pnt = (t_arrlist **)malloc(sizeof(t_arrlist *) * 1);
	ft_memset(wild->pnt, 0, sizeof(t_arrlist *) * 1);
	if (!wild->pnt)
	{
		free_arrlist(wild->list);
		perror(PMT);
		return (WILD_MALLOC_ERROR);
	}
	wild->pnt[0] = create_arrlist();
	wild->argc = 1;
	wild->argv = (char **)malloc(sizeof(char *) * 2);
	wild->argv[0] = ft_strdup(cmd->file);
	wild->argv[1] = NULL;
	return (0);
}

int	init_wildcard(t_wildcard *wild, t_execcmd *cmd)
{
	int	i;

	wild->list = create_arrlist();
	if (!wild->list)
	{
		perror(PMT);
		return (WILD_MALLOC_ERROR);
	}
	wild->pnt = (t_arrlist **)malloc(sizeof(t_arrlist *) * cmd->argc);
	ft_memset(wild->pnt, 0, sizeof(t_arrlist *) * cmd->argc);
	if (!wild->pnt)
	{
		free_arrlist(wild->list);
		perror(PMT);
		return (WILD_MALLOC_ERROR);
	}
	i = -1;
	while (++i < cmd->argc)
		wild->pnt[i] = create_arrlist();
	wild->argc = cmd->argc;
	wild->argv = cmd->argv;
	return (0);
}

int	free_wildcard(t_wildcard *wild, int clean_list, int error)
{
	int	i;

	if (!wild)
		return (error);
	if (clean_list && wild->list)
	{
		free_arrlist(wild->list);
		wild->list = NULL;
	}
	i = -1;
	if (!wild->pnt)
		return (error);
	while (++i < wild->argc)
	{
		if (wild->pnt[i])
		{
			free_arrlist(wild->pnt[i]);
			wild->pnt[i] = NULL;
		}
	}
	free(wild->pnt);
	return (error);
}

int	do_single_match(int i, t_wildcard *wild, char *str, DIR *dir)
{
	char	*pat;
	int		err;

	err = 0;
	pat = wild->argv[i];
	if (pat[0] == '.')
		err = match(pat, str) && add_string_arrlist(wild->pnt[i], str);
	else if (str[0] != '.')
		err = match(pat, str) && add_string_arrlist(wild->pnt[i], str);
	if (err)
	{
		perror(PMT);
		free_wildcard(wild, 1, 1);
		if (closedir(dir) == -1)
			return (WILD_MALLOC_ERROR | WILD_ERR_DIR);
		return (WILD_MALLOC_ERROR);
	}
	return (0);
}
