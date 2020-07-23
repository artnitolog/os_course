#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

enum { LIM = 4 };

volatile sig_atomic_t cur = 0, cnt = 0;

void hnd(int signo)
{
    if (signo == SIGTERM || ++cnt == LIM) {
        _exit(0);
    }
    printf("%d\n", cur);
    fflush(stdout);
}

int isprime(int n) {
    if (n == 2) {
        return 1;
    }
    if (n == 1 || !(n % 2)) {
        return 0;
    }
    int bnd = sqrt(n);
    for (int i = 3; i <= bnd; i += 2) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[])
{
    struct sigaction sa = { .sa_handler = hnd, .sa_flags = SA_RESTART };
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    int low, high;
    scanf("%d%d", &low, &high);
    for (int i = low; i < high; i++) {
        if (isprime(i)) {
            cur = i;
        }
    }
    printf("-1\n");
    fflush(stdout);
    return 0;
}
