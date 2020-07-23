#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>

int perform(const char *cmd)
{
    int pid = fork();
    if (pid < 0) {
        return 0;
    } else if (!pid) {
        execlp(cmd, cmd, NULL);
        _exit(1);
    }
    int status;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) && !WEXITSTATUS(status);
}

int main(int argc, char *argv[])
{
    int pid1, pid2;
    int fd[2];
    if (pipe(fd) < 0) {
        _exit(1);
    }
    if ((pid1 = fork()) < 0) {
        _exit(1);
    } else if (!pid1) {
        close(fd[0]);
        int fdr = open(argv[4], O_RDONLY);
        if (fdr < 0 || dup2(fdr, 0) < 0 || dup2(fd[1], 1) < 0) {
            _exit(1);
        }
        close(fd[1]);
        close(fdr);
        execlp(argv[1], argv[1], NULL);
        _exit(1);
    }
    close(fd[1]);
    if ((pid2 = fork()) < 0) {
        _exit(1);
    } else if (!pid2) {
        int fdw = open(argv[5], O_APPEND | O_CREAT | O_WRONLY, 0640);
        if (fdw < 0 || dup2(fd[0], 0) < 0 || dup2(fdw, 1) < 0) {
            _exit(1);
        }
        close(fdw);
        close(fd[0]);
        int res = perform(argv[2]) || perform(argv[3]);
        _exit(res);
    }
    close(fd[0]);
    while (wait(NULL) > 0);
    return 0;
}