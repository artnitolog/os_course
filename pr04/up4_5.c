#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

unsigned long long hash_function(unsigned long long);

int main(int argc, char *argv[])
{
    unsigned long long h;
    int cnt;
    int fd = open(argv[1], O_CREAT | O_WRONLY, 0660);
    if (fd == -1) {
        exit(1);
    }
    if (sscanf(argv[2], "%llx", &h) < 1) {
        exit(1);
    }
    if (sscanf(argv[3], "%d", &cnt) < 1) {
        exit(1);
    }
    off_t start = lseek(fd, 0, SEEK_END);
    if (start == -1) {
        exit(1);
    }
    for (int i = cnt - 1; i >= 0; i--) {
        if (lseek(fd, start + i * sizeof(h), SEEK_SET) == -1) {
            exit(1);
        }
        if (write(fd, &h, sizeof(h)) != sizeof(h)) {
            exit(1);
        }
        h = hash_function(h);
    }
    close(fd);
    return 0;
}