#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>

int main(int argc, char **argv)
{
    int fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0600);
    short unsigned cur;
    while (scanf("%hu", &cur) > 0) {
        unsigned char buf[2];
        buf[0] = cur >> 8;
        buf[1] = cur;
        write(fd, buf, sizeof(buf));
    }
    close(fd);
    return 0;
}