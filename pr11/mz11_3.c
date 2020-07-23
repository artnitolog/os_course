#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

void work(char *name)
{
    char s[PATH_MAX];
    FILE *fin = fopen(name, "r");
    fscanf(fin,"%4096s", s);
    fclose(fin);
    pid_t pid = fork();
    if (pid < 0) {
        return;
    } else if (!pid) {
        execl(s, s, NULL);
        _exit(1);
    }
}

int main(int argc, char *argv[])
{
    unsigned n = strtoul(argv[1], NULL, 10) + 1;
    n = argc - 1 < n ? argc - 1 : n;
    for (int i = 2; i <= n; i++) {
        work(argv[i]);
    }
    unsigned cnt = 0;
    int status;
    while (wait(&status) > 0) {
        cnt += WIFEXITED(status) && !WEXITSTATUS(status);
    }
    for (int i = n + 1; i < argc; i++) {
        work(argv[i]);
        wait(&status);
        cnt += WIFEXITED(status) && !WEXITSTATUS(status);
    }
    printf("%u\n", cnt);
    fflush(stdout);
    return 0;
}
