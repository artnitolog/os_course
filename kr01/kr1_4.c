#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <string.h>

enum
{
    BASE = 10
};

int main(int argc, char *argv[])
{
    char *path = argv[1];
    unsigned long long n = strtoull(argv[2], NULL, BASE);
    if (n < 1) {
        return 0;
    }
    int fd = open(path, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "%s\n", strerror(errno));
        return 0;
    }
    double prev, cur;
    ssize_t r = read(fd, &prev, sizeof(prev));
    if (r != sizeof(prev)) {
        close(fd);
        fprintf(stderr, "%s\n", strerror(errno));
        return 0;
    }
    for (unsigned long long i = 1; i < n; i++) {
        r = read(fd, &cur, sizeof(cur));
        if (r == 0) {
            break;
        }
        if (r != sizeof(cur)) {
            close(fd);
            fprintf(stderr, "%s\n", strerror(errno));
            return 0;
        }
        prev += cur;
        if (lseek(fd, -1 * (int) sizeof(cur), SEEK_CUR) < 0) {
            close(fd);
            fprintf(stderr, "%s\n", strerror(errno));
            return 0;
        }
        if (write(fd, &prev, sizeof(prev)) < sizeof(prev)) {
            close(fd);
            fprintf(stderr, "%s\n", strerror(errno));
            return 0;
        }
    }
    close(fd);
    return 0;
}
