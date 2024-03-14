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

// shopt -s dotglob  // modify bash behaviour 
int	wildcard_star(t_execcmd *cmd)
{
	DIR				*directory;
	struct dirent	*entry;
	
	t_arrlist	*list;
	t_arrlist	**p_list;
	
	int i;
	int j;
	
	char **argv_copy;
	argv_copy = cmd->argv;

	list = create_arrlist();
	p_list = (t_arrlist **)malloc(sizeof(t_arrlist *) * cmd->argc);
	i = -1;
	while (++i < cmd->argc)
		p_list[i] = create_arrlist();
	if (!list)
		return (1);

	directory = opendir(".");
	if (!directory)
	{
		perror("Unable to open directory");
		return (1);
	}

	entry = readdir(directory);
	while (entry)
	{
		i = -1;
		while (++i < cmd->argc)
		{
			if (match(cmd->argv[i], entry->d_name) && \
					add_string_arrlist(p_list[i], entry->d_name))
			{
				free_arrlist(list);
				return (1);
			}
		}
		entry = readdir(directory);
	}


	i = -1;
	while (++i < cmd->argc)
	{
		if (p_list[i]->size == 0)
			add_string_arrlist(p_list[i], cmd->argv[i]);
	}
	i = 0;
	while (i < cmd->argc)
	{
		j = 0;
		while (j < p_list[i]->size)
		{
			add_string_arrlist(list, p_list[i]->data[j]);
//    	ft_dprintf(2, "->%s<-\n", p_list[i]->data[j]);
			j++;
		}
		i++;
	}
	cmd->argv = list->data;
//	printf("free?\n");
	//ft_free_char2d(argv_copy);
	/*
	cmd->argv = list->data;
	free_arrlist(list);
*/
	closedir(directory);
	return (0);
}
