#include <stdio.h>
#include <dirent.h>


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


int	main(void)
{
	DIR				*directory;
	struct dirent	*entry;
	char *pattern = "test*.txt";

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
			printf("%s\n", entry->d_name);
		}
		entry = readdir(directory);
	}
	closedir(directory);
	return (0);
}

