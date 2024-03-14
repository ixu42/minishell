#include <stdio.h>
#include <dirent.h>

int main() {
    DIR *dir;
    struct dirent *entry;

    // Open the directory
    dir = opendir(".");
    if (dir == NULL) {
        perror("Unable to open directory");
        return 1;
    }

    // Read directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Determine file type
        const char *file_type;
        switch (entry->d_type) {
            case DT_BLK:
                file_type = "block device";
                break;
            case DT_CHR:
                file_type = "character device";
                break;
            case DT_DIR:
                file_type = "directory";
                break;
            case DT_FIFO:
                file_type = "named pipe (FIFO)";
                break;
            case DT_LNK:
                file_type = "symbolic link";
                break;
            case DT_REG:
                file_type = "regular file";
                break;
            case DT_SOCK:
                file_type = "UNIX domain socket";
                break;
            default:
                file_type = "unknown";
                break;
        }
        printf("%s:\t%s\n", entry->d_name, file_type);
    }

    // Close the directory
    closedir(dir);

    return 0;
}

