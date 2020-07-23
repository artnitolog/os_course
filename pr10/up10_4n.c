#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void blood(int n, pid_t *pid)
{
    for (int i = 0; i <= n; i++) {
        kill(pid[i], SIGKILL);
    }
    free(pid);
    _exit(1);
}

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        return 0;
    }
    int n = argc - 1;
    char **cmd = argv + 1;
    pid_t *pid = calloc(n, sizeof(*pid));
    int fd[2];
    int fdr = dup(0);
    for (int i = 0; i < n; i++) {
        if (pipe(fd) < 0) {
            blood(i - 1, pid);
        }
        if ((pid[i] = fork()) < 0) {
            close(fd[0]);
            close(fd[1]);
            blood(i - 1, pid);
        } else if (!pid[i]) {
            close(fd[0]);
            if (dup2(fdr, 0) < 0) {
                _exit(1);
            }
            close(fdr);
            if (i != n - 1 && dup2(fd[1], 1) < 0) {
                _exit(1);
            }
            close(fd[1]);
            execlp(cmd[i], cmd[i], NULL);
            _exit(1);
        }
        close(fd[1]);
        if (dup2(fd[0], fdr) < 0) {
            close(fd[0]);
            blood(i, pid);
        }
        close(fd[0]);
    }
    close(fdr);
    free(pid);
    while (wait(NULL) > 0);
    return 0;
}