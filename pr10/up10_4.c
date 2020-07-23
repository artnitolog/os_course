#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

void blood(pid_t *pid, int n)
{
    for (int i = 0; i < n; i++) {
        kill(pid[i], SIGKILL);
    }
    free(pid);
    while (wait(NULL) > 0);
    _exit(1);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        return 0;
    }
    pid_t *pid = calloc(argc - 1, sizeof(*pid));
    if (!pid) {
        _exit(1);
    }
    int fd[2];
    for (int i = 1; i < argc; i++) {
        if (pipe(fd) < 0) {
            blood(pid, i - 1);
        }
        pid[i - 1] = fork();
        if (pid[i - 1] < 0) {
            close(fd[0]);
            close(fd[1]);
            blood(pid, i - 1);
        } else if (!pid[i - 1]) {
            /* current child */
            close(fd[0]);
            if (i != argc - 1 && dup2(fd[1], 1) < 0) {
                _exit(1);
            }
            close(fd[1]);
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        }
        /* parent */
        close(fd[1]);
        if (dup2(fd[0], 0) < 0) {
            close(fd[0]);
            blood(pid, i);
        }
        close(fd[0]);
    }
    free(pid);
    while (wait(NULL) > 0);
    return 0;
}
