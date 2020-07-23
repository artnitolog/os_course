#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

void birth(void) {
    int n;
    if (scanf("%d", &n) != 1) {
        exit(0);
    }
    pid_t pr = fork();
    if (pr < 0) {
        fprintf(stderr, "fork: %s\n", strerror(errno));
        exit(1);
    } else if (pr == 0) {
        birth();
    } else {
        int status;
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("%d\n", n);
            fflush(stdout);
            exit(0);
        } else {
            exit(1);
        }
    }
}

int main(void)
{
    pid_t allfather = fork();
    if (allfather == -1) {
        fprintf(stderr, "fork: %s\n", strerror(errno));
        exit(0);
    } else if (allfather == 0) {
        birth();
    }
    int status;
    wait(&status);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        printf("-1\n");
    }
    return 0;
}