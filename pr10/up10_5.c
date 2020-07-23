#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

void pp(int owner, int max, int fdr, int fdw)
{
    FILE *fin = fdopen(fdr, "r");
    FILE *fout = fdopen(fdw, "w");
    if (!fin || !fout) {
        _exit(1);
    }
    while (1) {
        int cur;
        if (fscanf(fin, "%d", &cur) == EOF || max == 1) {
            break;w
        }
        printf("%d %d\n", owner, cur);
        fflush(stdout);
        if (++cur == max) {
            break;
        }
        fprintf(fout, "%d ", cur);
        fflush(fout);
    }
    fclose(fin);
    fclose(fout);
    close(fdr);
    close(fdw);
    _exit(0);
}

int main(int argc, char *argv[])
{
    int max = strtol(argv[1], NULL, 10);
    int fd12[2], fd21[2];
    if (pipe(fd12) == -1 || pipe(fd21) == -1) {
        return 1;
    }
    pid_t pid1, pid2;
    if ((pid1 = fork()) == -1) {
        return 1;
    } else if (pid1 == 0) {
        close(fd12[0]);
        close(fd21[1]);
        pp(1, max, fd21[0], fd12[1]);
    }
    if ((pid2 = fork()) == -1) {
        return 1;
    } else if (pid2 == 0) {
        close(fd12[1]);
        close(fd21[0]);
        pp(2, max, fd12[0], fd21[1]);
    }
    FILE *fout = fdopen(fd21[1], "w");
    fprintf(fout, "%d ", 1);
    fflush(fout);
    fclose(fout);
    close(fd12[0]);
    close(fd21[0]);
    close(fd12[1]);
    close(fd21[1]);
    while (wait(NULL) > 0);
    printf("Done\n");
    return 0;
}