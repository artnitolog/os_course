#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid1, pid2;
    int fd[2];
    if (pipe(fd) < 0) {
        _exit(1);
    }
    pid1 = fork();
    if (pid1 < 0) {
        _exit(1);
    } else if (!pid1) {
        /* ( cmd1 < file1 && cmd2 ) */
        close(fd[0]);
        if (dup2(fd[1], 1) < 0) {
            _exit(1);
        }
        close(fd[1]);
        int pid1_1, pid1_2;
        if ((pid1_1 = fork()) < 0) {
            _exit(1);
        } else if (!pid1_1) {
            /* cmd1 */
            int fdr = open(argv[4], O_RDONLY);
            if (fdr < 0|| dup2(fdr, 0) < 0) {
                _exit(1);
            }
            close(fdr);
            execlp(argv[1], argv[1], NULL);
            _exit(1);
        }
        int status;
        wait(&status);
        if (!WIFEXITED(status) || WEXITSTATUS(status)) {
            _exit(0);
        }
        if ((pid1_2 = fork()) < 0) {
            _exit(1);
        } else if (!pid1_2) {
            /* cmd2 */
            execlp(argv[2], argv[2], NULL);
            _exit(1);
        }
        wait(NULL);
        _exit(0);
    }
    close(fd[1]);
    if ((pid2 = fork()) < 0) {
        _exit(1);
    } else if (!pid2) {
        /* cmd3 */
        int fdw = open(argv[5], O_WRONLY | O_CREAT | O_APPEND, 0600);
        if (fdw < 0 || dup2(fdw, 1) < 0 || dup2(fd[0], 0) < 0) {
            _exit(1);
        }
        close(fd[0]);
        close(fdw);
        execlp(argv[3], argv[3], NULL);
        _exit(1);
    }
    close(fd[0]);
    while (wait(NULL) > 0);
    return 0;
}