#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <inttypes.h>
#include <stdio.h>

int main(void)
{
    int fd[2];
    if (pipe(fd)  == -1) {
        return 1;
    }
    pid_t pid1, pid2;
    if ((pid1 = fork()) == -1) {
        return 1;
    } else if (pid1 == 0) {
        /* child */
        close(fd[1]);
        if ((pid2 = fork()) == -1) {
            _exit(1);
        } else if (pid2 == 0) {
            /* grandchild */
            int32_t b;
            int64_t sum = 0;
            while (read(fd[0], &b, sizeof(b)) == sizeof(b)) {
                sum += b;
            }
            close(fd[0]);
            printf("%" PRId64 "\n", sum);
            fflush(stdout);
            return 0;
        }
        close(fd[0]);
        wait(NULL);
        _exit(0);
    }
    close(fd[0]);
    int32_t a;
    while (scanf("%" SCNd32, &a) > 0) {
        write(fd[1], &a, sizeof(a));
    }
    close(fd[1]);
    wait(NULL);
    return 0;
}