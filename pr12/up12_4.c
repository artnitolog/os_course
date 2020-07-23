#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <inttypes.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

enum
{
    BASE = 10,
};

void work(int semid, int num, int count)
{
    while (semop(semid, (struct sembuf[]) { { num, -1, 0 } }, 1) != -1) {
        int32_t cur;
        if (scanf("%" SCNd32, &cur) == EOF) {
            semctl(semid, 0, IPC_RMID);
            break;
        }
        printf("%d %" PRId32 "\n", num, cur);
        fflush(stdout);
        unsigned short mod = ((cur % count) + count) % count;
        if (semop(semid, (struct sembuf[]){ { mod, 1, 0 } }, 1) < 0) {
            fprintf(stderr, "semop: %s\n", strerror(errno));
            semctl(semid, 0, IPC_RMID);
            _exit(1);
        }
    }
    _exit(0);
}

int main(int argc, char *argv[])
{
    int count = strtol(argv[1], NULL, BASE);
    int semid = semget(IPC_PRIVATE, count, 0600 | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        fprintf(stderr, "semget: %s\n", strerror(errno));
    }
    setbuf(stdin, NULL);
    for (int i = 0; i < count; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork : %s\n", strerror(errno));
            semctl(semid, 0, IPC_RMID);
            _exit(1);
        } else if (!pid) {
            work(semid, i, count);
        }
    }
    semctl(semid, 0, SETVAL, 1);
    while (wait(NULL) > 0);
    return 0;
}