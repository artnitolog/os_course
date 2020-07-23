#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int fd1[2];
    pipe(fd1);
    if (!fork()) {
        close(fd1[0]);
        dup2(fd1[1], 1);
        close(fd1[1]);
        execlp(argv[1], argv[1], NULL);
        _exit(1);
    }
    close(fd1[1]);
    int fd2[2];
    pipe(fd2);
    if (!fork()) {
        close(fd2[0]);
        dup2(fd1[0], 0);
        close(fd1[0]);
        dup2(fd2[1], 1);
        close(fd2[1]);
        if (!fork()) {
            int file1 = open(argv[5], O_CREAT | O_TRUNC | O_WRONLY, 0600);
            dup2(file1, 2);
            close(file1);
            execlp(argv[2], argv[2], NULL);
            _exit(1);
        }
        int status;
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status)) {
            return 0;
        }
        if (!fork()) {
            execlp(argv[3], argv[3], NULL);
            _exit(1);
        }
        wait(NULL);
        return 0;
    }
    close(fd1[0]);
    close(fd2[1]);
    if (!fork()) {
        dup2(fd2[0], 0);
        close(fd2[0]);
        execlp(argv[4], argv[4], NULL);
        _exit(1);
    }
    close(fd2[0]);
    while (wait(NULL) > 0);
    return 0;
}
