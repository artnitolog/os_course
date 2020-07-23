#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

enum
{
    EX_STATUS = 42
};

int main(int argc, char *argv[])
{
    pid_t pid;
    if (!(pid = fork())) {
        int fd_read = open(argv[2], O_RDONLY);
        int fd_write = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, 0660);
        int fd_err = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0660);
        if (fd_read < 0 || fd_write < 0 || fd_err < 0) {
            _exit(EX_STATUS);
        }
        if (dup2(fd_read, STDIN_FILENO) < 0 || dup2(fd_write, STDOUT_FILENO) < 0
            || dup2(fd_err, STDERR_FILENO) < 0) {
            _exit(EX_STATUS);
        }
        close(fd_read);
        close(fd_write);
        close(fd_err);
        execlp(argv[1], argv[1], NULL);
        _exit(EX_STATUS);
    } else if (pid < 0) {
        fprintf(stderr, "fork: %s\n", strerror(errno));
        _exit(0);
    }
    int status;
    wait(&status);
    printf("%d\n", status);
    return 0;
}