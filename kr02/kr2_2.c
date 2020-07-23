#include <unistd.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int n = argc - 1;
    int *pid = calloc(n, sizeof(*pid));
    for (int i = 1; i < argc; i++) {
        if (!(pid[i - 1] = fork())) {
            int cnt = 0;
            uint64_t cur = strtoull(argv[i], NULL, 10);
            while (cur) {
                cnt += cur & 1;
                cur >>= 1;
            }
            _exit(cnt);
        }
    }
    int *ans = calloc(argc - 1, sizeof(*ans));
    for (int i = 0; i < n; i++) {
        int status;
        int child = wait(&status);
        if (!WIFEXITED(status)) {
            free(ans);
            free(pid);
            while (wait(NULL) > 0);
            _exit(1);
        }
        for (int j = 0; j < n; j++) {
            if (pid[j] == child) {
                ans[j] = WEXITSTATUS(status);
                break;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        printf("%d\n", ans[i]);
    }
    free(pid);
    free(ans);
    return 0;
}