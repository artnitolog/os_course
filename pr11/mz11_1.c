#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

volatile sig_atomic_t cnt = -1;

enum 
{
    LIM = 5
};

void hnd(int signo)
{
    if (++cnt == LIM) {
        _exit(0);
    } else {
        printf("%d\n", cnt);
        fflush(stdout);
    }
}

int main(void)
{
    sigaction(SIGHUP, &(struct sigaction) 
        { 
            .sa_handler = hnd, 
            .sa_flags = SA_RESTART 
        }, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    while (1) {
        pause();
    }
    return 0;
}
