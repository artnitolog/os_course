#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    char tmp[] = "/tmp";
    char *dir;
    if (!(dir = getenv("XDG_RUNTIME_DIR"))) {
        if (!(dir = getenv("TMPDIR"))) {
            dir = tmp;
        }
    }
    char pathname[PATH_MAX];
    memset(pathname, 0, sizeof(pathname));
    snprintf(pathname, sizeof(pathname), "%s/bagheera_%d.sh", dir, getpid());
    int fd = creat(pathname, 0744);
    if (fd < 0) {
        return 1;
    }
    FILE *f = fdopen(fd, "w");
    if (!f) {
        return 1;
    }
    fprintf(f, "#! /usr/bin/python3\n"
            "from sys import argv\n"
            "from os import unlink\n"
            "print(1");
    for (int i = 1; i < argc; i++) {
        fprintf(f, "*%s", argv[i]);
    }
    fprintf(f, ")\nunlink(argv[0])\n");
    fclose(f);
    close(fd);
    execl("/bin/sh", "sh", "-c", pathname, NULL);
    return 1;
}