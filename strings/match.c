# include "../minishell.h"

int	match(const char *pattern, const char *text)
{
	if (*pattern == '\0' && *text == '\0')
		return (1);
	//if (*pattern == '*i' && *(pattern + 1) != '\0' && *text == '\0')
	if (*pattern == ASCII_WILD && *(pattern + 1) != '\0' && *text == '\0')
		return (0);
	if (*pattern == *text)
		return (match(pattern + 1, text + 1));
	if (*pattern == ASCII_WILD)
		return (match(pattern + 1, text) || match(pattern, text + 1));
	return (0);
}

/*
typedef struct s_wildcard
{
    t_arrlist   *list;
    t_arrlist   **pnt;
    char        **argv;
    int         argc;
}   t_wildcard;
*/

int	init_wildcard_redir(t_wildcard *wild, t_redircmd *cmd)
{
	int	i;

	wild->list = create_arrlist();
	if (!wild->list)
		return (1);
	wild->pnt = (t_arrlist **)malloc(sizeof(t_arrlist *) * 1);
	ft_memset(wild->pnt, 0, sizeof(t_arrlist *) * 1);
	if (!wild->pnt)
	{
		free_arrlist(wild->list);
		return (1);
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
		return (1);
	wild->pnt = (t_arrlist **)malloc(sizeof(t_arrlist *) * cmd->argc);
	ft_memset(wild->pnt, 0, sizeof(t_arrlist *) * cmd->argc);
	if (!wild->pnt)
	{
		free_arrlist(wild->list);
		return (1);
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
//	printf("pattern =%s<-\n", pat);
	/*
	if (!ft_strchr(pat, ASCII_WILD))
		return (0);
	*/
	if (pat[0] == '.')
		err = match(pat, str) && add_string_arrlist(wild->pnt[i], str);
	else if (str[0] != '.')
		err = match(pat, str) && add_string_arrlist(wild->pnt[i], str);
	if (err)
	{
		if (closedir(dir) == -1)
			return (1);
		free_wildcard(wild, 1, 1);
		return (1);
	}
	return (0);
}

// readdir and write matches to p_list given cmd->argv
int	match_to_files(t_wildcard *wild)
{
	DIR				*directory;
	struct dirent	*entry;
	int				i;

	directory = opendir(".");
	if (!directory)
	{
		perror("Unable to open directory");
		return (2);
	}
	entry = readdir(directory);
	while (entry)
	{
		i = -1;
		while (++i < wild->argc)
		{
			if (!ft_strchr(wild->argv[i], ASCII_WILD))
				continue ;
			if (do_single_match(i, wild, entry->d_name, directory))
				return (1);
		}
		entry = readdir(directory);
	}
	closedir(directory);
	return (0);
}

void	replace_str(char *s, char from, char to)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == from)
		{
//			printf("->%c<-, c=%d, space=%d\n", from, (int)from, (int)' ');
			s[i] = to;
		}
		i++;
	}
}

int	make_sorted_argv(t_wildcard *wild, int be_sorted)
{
	int				i;
	int				j;
	t_arrlist	*arr;

	i = -1;
	while (++i < wild->argc)
	{
		arr = wild->pnt[i];
		if (arr->size == 0)
		{
			//replace_str(arr->data[0]
//			printf("size == 0\n");
			replace_str(wild->argv[i], ASCII_WILD, '*');
			if (add_string_arrlist(arr, wild->argv[i]))
				return (free_wildcard(wild, 1, 1));
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
	int				i;
	int				j;
	t_arrlist	*arr;

	i = -1;
	while (++i < wild->argc)
	{
		arr = wild->pnt[i];
		j = -1;
	//	printf("i = %d, arr size =%d\n",i, (int)arr->size);
		while (++j < arr->size)
		{
//			printf("arg=%s\n",arr->data[j]);
			if (add_string_arrlist(wild->list, arr->data[j]))
				return (free_wildcard(wild, 1, 1));
		}
		//printf("arg=%s\n",arr->data[j]);
	}
	return (0);
}

// shopt -s dotglob  // modify bash behaviour 
int	wildcard_star(t_execcmd *cmd)
{
	t_wildcard	wild;

	if (init_wildcard(&wild, cmd))
		return (1);
	if (match_to_files(&wild))
		return (2);
	if (make_sorted_argv(&wild, 1))
		return (3);
	if (copy_sorted_argv(&wild))
		return (4);
	// ??? do we need free(cmd->argv);  ???
	ft_free_char2d(cmd->argv);
	// free(cmd->argv);  lead to double free
	cmd->argv = wild.list->data;
	cmd->argc = wild.argc;
	cmd->list = wild.list;
	free_wildcard(&wild, 0, 0);
//	ft_free_char2d(wild.argv);
	return (0);
}

int	wildcard_star_redir(t_redircmd *cmd)
{
	t_wildcard	wild;

	if (init_wildcard_redir(&wild, cmd))
		return (1);
	if (match_to_files(&wild))
		return (2);
	if (make_sorted_argv(&wild, 0))
		return (3);
	if (copy_sorted_argv(&wild))
		return (3);
	if (wild.pnt[0]->size > 1)
	{
		ft_dprintf(2, "%s %s %s\n", PMT, cmd->sfile, ERR_REDIR_AMBIG);
		return (4);
	}
	cmd->list = wild.list;
	free(cmd->file);
	cmd->file = wild.list->data[0];
	free_wildcard(&wild, 0, 0);
	ft_free_char2d(wild.argv);
//	free(wild.argv);
	wild.argv = NULL;
	return (0);
}
