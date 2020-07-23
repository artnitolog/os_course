#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>

enum
{
    NAME_SIZE = 16,
    CONVERSION = 100
};

struct Entry
{
    char name[NAME_SIZE];
    int32_t cash;
};

int main(int argc, char *argv[])
{
    int32_t max1 = 0, max2 = 0;
    int flag1 = 0, flag2 = 0;
    struct Entry entry;
    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd == -1) {
            continue;
        }
        while (read(fd, &entry, sizeof(entry)) == sizeof(entry)) {
            int32_t cur = entry.cash;
            if (!flag1 || cur >= max1) {
                if (flag1 && max1 != cur) {
                    flag2 = 1;
                    max2 = max1;
                }
                flag1 = 1;
                max1 = cur;
            } else if (!flag2 || cur > max2) {
                flag2 = 1;
                max2 = cur;
            }
        }
        close(fd);
    }
    if (flag2) {
        double res = max2;
        res /= CONVERSION;
        printf("%.2lf\n", res);
    }
    return 0;
}
