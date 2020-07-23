#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        exit(errno);
    }
    off_t min_pos = 0, cur_pos = 0;
    long long min_val, cur_val;
    ssize_t read_ret = read(fd, &min_val, sizeof(min_val));
    if (read_ret < sizeof(min_val)) {
        close(fd);
        exit(errno);
    }
    while (read(fd, &cur_val, sizeof(cur_val)) == sizeof(cur_val)) {
        cur_pos += sizeof(cur_val);
        if (cur_val < min_val) {
            min_val = cur_val;
            min_pos = cur_pos;
        }
    }
    min_val = -(unsigned long long)min_val;
    if (lseek(fd, min_pos, SEEK_SET) < 0 ||
            write(fd, &min_val, sizeof(min_val)) < 0) {
        close(fd);
        exit(errno);
    }
    close(fd);
    return 0;
}