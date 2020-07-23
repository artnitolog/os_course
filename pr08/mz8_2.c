#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    pid_t grandparent, parent;

    if ((grandparent = fork()) == 0) {
        if ((parent = fork()) == 0) {
            printf("3 ");
            exit(0);
        } else if (parent > 0) {
            wait(NULL);
            printf("2 ");
            exit(0);
        } else {
            fprintf(stderr, "fork: %s\n", strerror(errno));
            exit(errno);
        }
    } else if (grandparent > 0) {
        wait(NULL);
        printf("1\n");
        exit(0);
    } else {
        fprintf(stderr, "fork: %s\n", strerror(errno));
        exit(errno);
    }
}