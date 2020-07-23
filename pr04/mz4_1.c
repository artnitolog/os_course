#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

enum
{
    NAME_OFFSET = 16,
    CONVERSION = 100
};

int main(int argc, char *argv[])
{
    int max1, max2;
    int flag1 = 0, flag2 = 0;
    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd == -1) {
            continue;
        }
        while (lseek(fd, NAME_OFFSET, SEEK_CUR) != -1) {
            int cur;
            if (read(fd, &cur, sizeof(cur)) != sizeof(cur)) {
                break;
            }
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