#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char *argv[])
{
    unsigned num = strtoul(argv[2], NULL, 10);
    unsigned long long sum = 0;
    DIR *d = opendir(argv[1]);
    if (!d) {
        exit(errno);
    }
    char path[PATH_MAX];
    struct dirent *dd;
    struct stat stb;
    while ((dd = readdir(d)) != NULL) {
        if (!strcmp(".", dd->d_name) || !strcmp("..", dd->d_name) ||
            snprintf(path, sizeof(path), "%s/%s", argv[1], dd->d_name) >= sizeof(path)) {
            continue;
        }
        if (lstat(path, &stb) < 0 || !S_ISREG(stb.st_mode) || access(path, W_OK)) {
            continue;
        }
        int fd = open(path, O_RDONLY);
        if (fd < 0) {
            continue;
        }
        unsigned cur;
        if (read(fd, &cur, sizeof(cur)) != sizeof(cur)) {
            close(fd);
            continue;
        }
        if (cur == num) {
            sum += stb.st_size;
        }
        close(fd);
    }
    closedir(d);
    printf("%llu\n", sum);
    return 0;
}
