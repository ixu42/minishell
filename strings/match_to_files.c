/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_to_files.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 11:15:56 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/24 11:26:11 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	panic_wildcard(t_wildcard *wild)
{
	perror("Unable to open directory");
	free_wildcard(wild, 1, 1);
	return (WILD_ERR_DIR);
}

// readdir and write matches to p_list given cmd->argv
int	match_to_files(t_wildcard *wild)
{
	struct dirent	*entry;
	int				i;
	int				err;

	err = 0;
	entry = readdir(wild->directory);
	while (entry)
	{
		i = -1;
		while (++i < wild->argc)
		{
			if (!ft_strchr(wild->argv[i], ASCII_WILD))
				continue ;
			err = do_single_match(i, wild, entry->d_name, wild->directory);
			if (err)
				return (err);
		}
		entry = readdir(wild->directory);
	}
	if (closedir(wild->directory) == -1)
		return (panic_wildcard(wild));
	return (0);
}

void	replace_str(char *s, char from, char to)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == from)
			s[i] = to;
		i++;
	}
}

int	make_sorted_argv(t_wildcard *wild, int be_sorted)
{
	int			i;
	t_arrlist	*arr;

	i = -1;
	while (++i < wild->argc)
	{
		arr = wild->pnt[i];
		if (arr->size == 0)
		{
			replace_str(wild->argv[i], ASCII_WILD, '*');
			if (add_string_arrlist(arr, wild->argv[i]))
				return (free_wildcard(wild, 1, WILD_MALLOC_ERROR));
		}
		else
		{
			if (be_sorted)
				heapsort_str(arr->data, arr->size);
		}
	}
	return (0);
}

int	copy_sorted_argv(t_wildcard *wild)
{
	int			i;
	int			j;
	t_arrlist	*arr;

	i = -1;
	while (++i < wild->argc)
	{
		arr = wild->pnt[i];
		j = -1;
		while (++j < (int)arr->size)
		{
			if (add_string_arrlist(wild->list, arr->data[j]))
			{
				perror(PMT);
				return (free_wildcard(wild, 1, WILD_MALLOC_ERROR));
			}
		}
	}
	return (0);
}
