# include "../minishell.h"

int	match(const char *pattern, const char *text)
{
	if (*pattern == '\0' && *text == '\0')
		return (1);
	if (*pattern == '*' && *(pattern + 1) != '\0' && *text == '\0')
		return (0);
	if (*pattern == *text)
		return (match(pattern + 1, text + 1));
	if (*pattern == '*')
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
// init dynamic arrays
int	init_wildcard(t_wildcard *wild, t_execcmd *cmd)
{
	int	i;

	wild->list = create_arrlist();
	if (!wild->list)
		return (1);
	wild->pnt = (t_arrlist **)malloc(sizeof(t_arrlist *) * cmd->argc);
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
	return  (0);
}

void	free_wildcard(t_wildcard *wild)
{
	int	i;

	free_arrlist(wild->list);
	i = -1;
	while (++i < wild->argc)
		free_arrlist(wild->pnt[i]);
}

int do_single_match(int	i, t_wildcard *wild, char *str, DIR *dir)
{
	// deal with .files and . and .. directories
	if (match(wild->argv[i], str) && \
				add_string_arrlist(wild->pnt[i], str))
	{
		closedir(dir);
		free_wildcard(wild);
		return (1);
	}
	return (0);
}

// readdir and write matchees to p_list given cmd->argv
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
			if (do_single_match(i, wild, entry->d_name, directory))
				return (1);
		}
		entry = readdir(directory);
	}
	closedir(directory);
	return (0);
}
		
// shopt -s dotglob  // modify bash behaviour 
int	wildcard_star(t_execcmd *cmd)
{
	t_wildcard	wild;

	int i;
	int j;
	

	if (init_wildcard(&wild, cmd))
		return (1);
	if (match_to_files(&wild))
		return (1);
	
	t_arrlist	*list;
	t_arrlist	**p_list;
	char **argv_copy;
	argv_copy = wild.argv;
	list = wild.list;
	p_list = wild.pnt;
	

// sort and fill empty
	i = -1;
	while (++i < cmd->argc)
	{
		if (p_list[i]->size == 0)
			add_string_arrlist(p_list[i], cmd->argv[i]);
		else
			heapsort_str(p_list[i]->data, p_list[i]->size);
	}
//  copy to list
	i = -1;
	while (++i < cmd->argc)
	{
		j = -1;
		while (++j < p_list[i]->size)
			add_string_arrlist(list, p_list[i]->data[j]);
	}
	cmd->argv = list->data;
//	printf("free?\n");
	//ft_free_char2d(argv_copy);
	/*
	cmd->argv = list->data;
	free_arrlist(list);
*/
	return (0);
}
