#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>

enum
{
    BASE4 = 4,
    BASE10 = 10,
};

int sum4(int64_t a)
{
    int sum = 0;
    while (a) {
        sum += a % BASE4;
        a /= BASE4;
    }
    return sum > 0 ? sum : -sum;
}

int main(int argc, char *argv[])
{
    int n = argc - 1;
    char **num = &argv[1];
    pid_t *pid = calloc(n, sizeof(*pid));
    for (int i = 0; i < n; i++) {
        pid[i] = fork();
        if (pid[i] < 0) {
            free(pid);
            return 1;
        } else if (!pid[i]) {
            int64_t cur = strtoll(num[i], NULL, BASE10);
            exit(sum4(cur));
        }
    }
    for (int i = 0; i < n; i++) {
        int ans;
        waitpid(pid[i], &ans, 0);
        if (WIFEXITED(ans)) {
            printf("%d\n", WEXITSTATUS(ans));
        } else {
            free(pid);
            return 1;
        }
    }
    free(pid);
    return 0;
}
