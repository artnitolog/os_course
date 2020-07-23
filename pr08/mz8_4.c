#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

int main(void)
{
    pid_t allfather = getpid();
    int n;
    scanf("%d", &n);
    printf("1");
    fflush(stdout);
    pid_t pr;
    for (int i = 2; i <= n; i++) {
        pr = fork();
        if (pr < 0) {
            fprintf(stderr, "fork: %s\n", strerror(errno));
            exit(errno);
        } else if (pr == 0) {
            printf(" %d", i);
            fflush(stdout);
        } else {
            wait(NULL);
            break;
        }
    }
    if (getpid() != allfather) {
        exit(0);
    }
    printf("\n");
    return 0;
}