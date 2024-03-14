#include "../minishell.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	size_t	copy_size;
	void		*new_ptr;
	
	if (!ptr)
		return (malloc(new_size));
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (old_size < new_size)
		copy_size = old_size;
	else
		copy_size = new_size;
	ft_memcpy(new_ptr, ptr, copy_size);
	free(ptr);
	return new_ptr;
}

/*
typedef struct s_arrlist
{
	 char		**data;
	 size_t	size;
	 size_t	capacity;
} t_arrlist;
*/

t_arrlist	*create_arrlist(void)
{
	t_arrlist	*list;
	list = (t_arrlist *)malloc(sizeof(*list));
	if (!list)
		return (NULL);
	list->data = (char**)malloc(INITIAL_CAPACITY * sizeof(char*));
	if (list->data == NULL)
	{
		free(list);
		return (NULL);
	}
	ft_memset(list->data, 0, INITIAL_CAPACITY);
	list->size = 0;
	list->capacity = INITIAL_CAPACITY;
	return list;
}

int	add_string_arrlist(t_arrlist *list, const char* str)
{
	size_t	capacity;
	if (list->size >= list->capacity - 1)
	{
		capacity = list->capacity * GROWTH_FACTOR;
		list->data = (char**)ft_realloc(list->data, \
			list->capacity * sizeof(char*), capacity * sizeof(char*));
		list->capacity = capacity;
		if (list->data == NULL)
			return (1);
	}
	list->data[list->size] = ft_strdup(str); // duplicate the string
	if (list->data[list->size] == NULL)
		return (1);
	list->size++;
	return (0);
}

void free_arrlist(t_arrlist *list)
{
	size_t	i;

	i = 0;
	while (i < list->size)
	{
		if (list->data[i])
		{
			free(list->data[i]);
			list->data[i] = NULL;
		}
		i++;
	}
	free(list->data);
	list->data = NULL;
	free(list);
}

/*
int	main(void)
{
	DIR				*directory;
	struct dirent	*entry;
	char *pattern = "test*.txt";
	t_arrlist	*list;
	list = create_arrlist();

	directory = opendir(".");
	if (!directory)
	{
		perror("Unable to open directory");
		return (1);
	}
	entry = readdir(directory);
	while (entry)
	{
		if (entry->d_type == DT_REG)
		{
			if (match(pattern,entry->d_name))
				add_string_arrlist(list, entry->d_name);
//			printf("%s\n", entry->d_name);
		}
		entry = readdir(directory);
	}

    for (size_t i = 0; i < list->capacity; i++) {
        printf("->%s<-\n", list->data[i]);
    }
//    printf("\n");
	free_arrlist(list);
	closedir(directory);
	return (0);
}
*/
