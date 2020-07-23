#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

void tree(int n, int cur) {
    if (n == 0) {
        printf("%d\n", cur);
        fflush(stdout);
        _exit(0);
    }
    if (!fork()) {
        tree(n - 1, 2 * cur + 1);
        wait(NULL);
        _exit(0);
    }
    wait(NULL);
    printf("%d\n", cur);
    fflush(stdout);
    if (!fork()) {
        tree(n - 1, 2 * cur + 2);
        wait(NULL);
        _exit(0);
    }
    wait(NULL);
    _exit(0);
}

int main(int argc, char *argv[])
{
    int n = strtol(argv[1], NULL, 10);
    tree(n, 0);
    return 0;
}