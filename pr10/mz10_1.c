#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>

int main(void)
{
    int fd[2];
    if (pipe(fd) == -1) {
        _exit(1);
    }
    time_t t;
    struct tm *ctime;
    pid_t pid1 = fork();
    if (pid1 == -1) {
        _exit(1);
    } else if (pid1 == 0) {
        pid_t pid2 = fork();
        if (pid2 == -1) {
            _exit(1);
        } else if (!pid2) {
            pid_t pid3 = fork();
            if (pid3 == -1 ) {
                _exit(1);
            } else if (pid3 == 0) {
                /* grandgrandchild */
                close(fd[0]);
                t = time(NULL);
                for (int i = 0; i < 3; i++) {
                    write(fd[1], &t, sizeof(t));
                }
                close(fd[1]);
                _exit(0);
            }
            /* grandchild */
            close(fd[1]);
            wait(NULL);
            if (read(fd[0], &t, sizeof(t)) != sizeof(t)) {
                _exit(1);
            }
            close(fd[0]);
            ctime = localtime(&t);
            printf("D:%02d\n", ctime->tm_mday);
            fflush(stdout);
            _exit(0);
        }
        /* child */
        close(fd[1]);
        wait(NULL);
        if (read(fd[0], &t, sizeof(t)) != sizeof(t)) {
            _exit(1);
        }
        close(fd[0]);
        ctime = localtime(&t);
        printf("M:%02d\n", ctime->tm_mon + 1);
        fflush(stdout);
        _exit(0);
    }
    /* father */
    close(fd[1]);
    wait(NULL);
    if (read(fd[0], &t, sizeof(t)) != sizeof(t)) {
        _exit(1);
    }
    close(fd[0]);
    ctime = localtime(&t);
    printf("Y:%04d\n", ctime->tm_year + 1900);
    fflush(stdout);
    return 0;
}