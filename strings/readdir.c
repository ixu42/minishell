#include <stdio.h>
#include <dirent.h>

int	main(void)
{
	DIR				*directory;
	struct dirent	*entry;

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
			printf("%s\n", entry->d_name);
		entry = readdir(directory);
	}
	closedir(directory);
	return (0);
}

