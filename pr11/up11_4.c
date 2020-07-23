#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

enum { BASE = 10 };

volatile sig_atomic_t flag = 0;

void hnd(int signo)
{
    flag = 1;
}

void pingpong(int id, int fdr, int fdw, int max)
{
    FILE *fin = fdopen(fdr, "r");
    FILE *fout = fdopen(fdw, "w");
    pid_t dest;
    sigset_t mask;
    sigemptyset(&mask);
    while (1) {
        while (!flag) {
            sigsuspend(&mask);
        }
        flag = 0;
        int cur;
        fscanf(fin, "%d%d", &dest, &cur);
        if (cur == max) { // from the father (the end)
            break;
        }
        printf("%d %d\n", id, cur++);
        fflush(stdout);
        if (cur == max) { // the first child to die
            break;
        }
        fprintf(fout, "%d %d\n", getpid(), cur);
        fflush(fout);
        kill(dest, SIGUSR1);
    }
    fclose(fin);
    fclose(fout);
    close(fdr);
    close(fdw);
    _exit(0);
}

int main(int argc, char *argv[])
{
    int max = strtol(argv[1], NULL, BASE);
    if (max <= 1) {
        printf("Done\n");
        return 0;
    }
    int fd[2];
    if (pipe(fd) < 0) {
        return 1;
    }
    sigaction(SIGUSR1, &(struct sigaction){ .sa_handler = hnd }, NULL);
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, NULL);
    pid_t pid1 = fork();
    if (pid1 < 0) {
        return 1;
    } else if (!pid1) {
        pingpong(1, fd[0], fd[1], max);
    }
    pid_t pid2 = fork();
    if (pid2 < 0) {
        return 1;
    } else if (!pid2) {
        pingpong(2, fd[0], fd[1], max);
    }
    close(fd[0]);
    FILE *fout = fdopen(fd[1], "w");
    fprintf(fout, "%d %d\n", pid2, 1);
    fflush(fout);
    kill(pid1, SIGUSR1);
    pid_t pid = wait(NULL);
    fprintf(fout, "%d %d\n", 0, max);
    fflush(fout);
    fclose(fout);
    close(fd[1]);
    pid ^= pid1 ^ pid2;
    kill(pid, SIGUSR1);
    wait(NULL);
    printf("Done\n");
    return 0;
}