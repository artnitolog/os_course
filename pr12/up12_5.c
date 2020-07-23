#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/wait.h>
#include <inttypes.h>

enum
{
    BUF_SIZE = 1000,
    MDA = 2
};

int birth(int new_lvl, uint32_t new_num, int old_fd, int bnd)
{
    int new_fd[2];
    pipe(new_fd);
    pid_t r = fork();
    if (r < 0) {
        printf("=(\n");
        exit(1);
    } else if (!r) {
        close(old_fd);
        close(new_fd[0]);
        birth(new_lvl, new_num, new_fd[1], bnd);
    }
    close(new_fd[1]);
    return new_fd[0];
}

void work(int lvl, uint32_t num, int fdw, int bnd) {
    if (!lvl || num > bnd) {
        _exit(MDA);
    }
    int left = birth(lvl - 1, (num << 1), fdw, bnd);
    int right = birth(lvl - 1, (num << 1) + 1, fdw, bnd);
    int status;
    wait(&status);
    if (WIFEXITED(status) && WEXITSTATUS(status) == MDA)

}


if (!lvl ||

int main(int argc, char *argv[])
{

    return 0;
}