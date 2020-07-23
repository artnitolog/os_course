#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>

enum
{
    SHIFT = 8
};

int main(int argc, char *argv[])
{
    uint16_t min = UINT16_MAX, cur;
    char buf[sizeof(uint16_t)];
    int exist = 0;
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        exit(errno);
    }
    while (read(fd, buf, sizeof(buf)) == sizeof(buf)) {
        cur = ((unsigned char) buf[0] << SHIFT) | ((unsigned char) buf[1]);
        if ((cur & 1) == 0) {
            exist = 1;
            min = cur < min ? cur : min;
        }
    }
    if (exist) {
        printf("%" PRIu16 "\n", min);
    }
    close(fd);
    return 0;
}