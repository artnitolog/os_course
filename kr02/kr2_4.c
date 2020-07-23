#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc < 3) {
        return 0;
    }
    char **cur_beg = &argv[argc];
    while (cur_beg != &argv[1]) {
        cur_beg[0] = NULL;
        cur_beg--;
        while (strcmp(cur_beg[0], argv[1])) {
            cur_beg--;
        }
        int pid = fork();
        if (pid < 0) {
            _exit(1);
        } else if (!pid) {
            execvp(cur_beg[1], &cur_beg[1]);
            _exit(1);
        }
        int status;
        wait(&status);
        if (!WIFEXITED(status) || WEXITSTATUS(status)) {
            _exit(1);
        }
    }
    return 0;
}