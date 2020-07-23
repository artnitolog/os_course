#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int fd13[2];
    pipe(fd13);
    if (!fork()) {
        close(fd13[0]);
        int fd23[2];
        pipe(fd23);
        if (!fork()) {
            close(fd23[1]);
            double rcvd, sum = 0;
            int check = 0;
            while (read(fd23[0], &rcvd, sizeof(rcvd)) == sizeof(rcvd)) {
                sum += rcvd;
                check = 1;
            }
            close(fd23[0]);
            if (check) {
                write(fd13[1], &sum, sizeof(sum));
            }
            close(fd13[1]);
            _exit(0);
        }
        close(fd13[1]);
        close(fd23[0]);
        FILE *fin = fopen(argv[1], "r");
        double snd;
        while (fscanf(fin, "%lf", &snd) == 1) {
            if (snd >= 0) {
                write(fd23[1], &snd, sizeof(snd));
            }
        }
        close(fd23[1]);
        wait(NULL);
        _exit(0);
    }
    close(fd13[1]);
    wait(NULL);
    double res;
    if (read(fd13[0], &res, sizeof(res)) == sizeof(res)) {
        printf("%.10g\n", res);
    }
    close(fd13[0]);
    return 0;
}