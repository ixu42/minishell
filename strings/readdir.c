#include <stdio.h>
#include <dirent.h>

int main() {
	DIR *directory;
	struct dirent *entry;

	directory = opendir(".");
	if (directory == NULL) {
		perror("Unable to open directory");
		return 1;
	}
	while ((entry = readdir(directory)) != NULL) 
	{
		if (entry->d_type == DT_REG)
			printf("%s\n", entry->d_name);
	}
	closedir(directory);
	return 0;
}

