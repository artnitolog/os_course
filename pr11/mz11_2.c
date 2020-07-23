#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <inttypes.h>
#include <signal.h>

volatile sig_atomic_t flag = SIGINT;

void hnd(int signo)
{
    flag = signo;
}

int main(void)
{
    struct sigaction sa = { .sa_handler = hnd, .sa_flags = SA_RESTART };
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    int32_t sum = 0;
    while (1) {
        int32_t cur;
        if (scanf("%" SCNd32, &cur) == EOF) {
            break;
        }
        if (flag == SIGINT) {
            __builtin_add_overflow(sum, cur, &sum); 
        } else {
            __builtin_mul_overflow(sum, cur, &sum);
        }
        printf("%" PRId32 "\n", sum);
        fflush(stdout);
    }    
    return 0;
}
