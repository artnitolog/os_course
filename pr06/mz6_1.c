#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    struct stat stbuf;
    unsigned long long sum = 0;
    for (int i = 1; i < argc; i++) {
        if (!lstat(argv[i], &stbuf) && S_ISREG(stbuf.st_mode)
                && stbuf.st_nlink == 1) {
            sum += stbuf.st_size;
        }
    }
    printf("%llu\n", sum);
    return 0;
}
