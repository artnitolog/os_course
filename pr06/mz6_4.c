#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    DIR *d = opendir(argv[1]);
    if (!d) {
        exit(errno);
    }
    uid_t who = getuid();
    struct dirent *cur_file;
    char s[PATH_MAX];
    struct stat stb;
    unsigned long long sum = 0;
    while ((cur_file = readdir(d))) {
        if (isupper(cur_file->d_name[0]) &&
                snprintf(s, sizeof(s), "%s/%s", argv[1], cur_file->d_name) < sizeof(s) &&
                !stat(s, &stb) && S_ISREG(stb.st_mode) && stb.st_uid == who) {
            sum += stb.st_size;
        }
    }
    closedir(d);
    printf("%llu\n", sum);
    return 0;
}