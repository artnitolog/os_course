#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <inttypes.h>

char **name;

void opener(int left, int right, int fdw);
int getfdr(int left, int right);
void intersect(FILE *f1, FILE *f2, int fdw);

int main(int argc, char *argv[])
{
    name = argv;
    int fd = getfdr(1, argc - 1);
    FILE *fin = fdopen(fd, "r");
    int32_t cur;
    while (fscanf(fin, "%" SCNd32, &cur) != EOF) {
        printf("%" PRId32 "\n", cur);
    }
    fclose(fin);
    close(fd);
    wait(NULL);
    return 0;
}

inline int32_t min(int32_t a, int32_t b)
{
    return a < b ? a : b;
}

void intersect(FILE *f1, FILE *f2, int fdw)
{
    FILE *fw = fdopen(fdw, "w");
    int32_t a, b;
    int r = min(fscanf(f1, "%" SCNd32, &a), fscanf(f2, "%" SCNd32, &b));
    while (r != EOF) {
        if (a == b) {
            fprintf(fw, "%" PRId32 "\n", a);
            r = min(fscanf(f1, "%" SCNd32, &a), fscanf(f2, "%" SCNd32, &b));
        } else if (a < b) {
            r = fscanf(f1, "%" SCNd32, &a);
        } else {
            r = fscanf(f2, "%" SCNd32, &b);
        }
    }
    fclose(fw);
    close(fdw);
    fclose(f1);
    fclose(f2);
    while (wait(NULL) > 0);
    _exit(0);
}

int getfdr(int left, int right)
{
    int fd[2];
    pipe(fd);
    if (!fork()) {
        close(fd[0]);
        opener(left, right, fd[1]);
    }
    close(fd[1]);
    return fd[0];
}

void opener(int left, int right, int fdw)
{
    int mid = (right + left) / 2;
    if (mid == left) {
        FILE *f1 = fopen(name[left], "r");
        FILE *f2 = fopen(name[right], "r");
        intersect(f1, f2, fdw);
    } else if (mid + 1 == right) {
        int newfd[2];
        pipe(newfd);
        if (!fork()) {
            close(newfd[0]);
            FILE *f1 = fopen(name[left], "r");
            FILE *f2 = fopen(name[left + 1], "r");
            intersect(f1, f2, newfd[1]);
        }
        close(newfd[1]);
        FILE *newfw = fdopen(newfd[0], "r");
        FILE *f3 = fopen(name[right], "r");
        intersect(newfw, f3, fdw);
    } else {
        int fd1 = getfdr(left, mid);
        int fd2 = getfdr(mid + 1, right);
        FILE *f1 = fdopen(fd1, "r");
        FILE *f2 = fdopen(fd2, "r");
        intersect(f1, f2, fdw);
    }
}