#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include <sys/wait.h>

enum
{
    NUM = 3,
    LEN = 8,
    BASE = 10
};

int main(void)
{
    pid_t pr;
    for (int i = 1; i <= NUM; i++) {
        if ((pr = fork()) < 0) {
            fprintf(stderr, "fork: %s\n", strerror(errno));
            exit(errno);
        } else if (pr == 0) {
            int32_t a;
            char buf[LEN];
            if (read(STDIN_FILENO, buf, sizeof(buf)) != sizeof(buf)) {
                fprintf(stderr, "read: %s\n", strerror(errno));
                exit(errno);
            }
            a = strtol(buf, NULL, BASE);
            printf("%d %" PRId32 "\n", i, a * a);
            fflush(stdout);
            exit(0);
        }
    }
    while (wait(NULL) != -1);
    return 0;
}