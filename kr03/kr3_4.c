#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

enum { N = 3 };

void work(int semid, uint32_t *a, int id)
{
    while (semop(semid, (struct sembuf[]) { { .sem_num = id, .sem_op = -1} }, -1) == 0) {
        uint32_t res = *a & ((1 << (8 * id)) - 1);
        printf("%d %" PRIx32 "\n", res);
        semop(semid, (struct sembuf[]) { { .sem_num = 0, .sem_op = -1} }, 1);
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    int semid = semget(IPC_PRIVATE, N + 1, 0);
    if (semid < 0) {
        return 1;
    }
    uint32_t *vptr = mmap(NULL, sizeof(*vptr), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if (vptr == MAP_FAILED) {
        semctl(semid, 0, IPC_RMID);
        return 1;
    }
    for (int i = 1; i <= N; i++) {
        if (!fork()) {
            work(semid, vptr, i);
        }
    }
    uint32_t ipget;
    semctl(semid, 0, SETVAL, 1);
    while (scanf("%" SCNo32, &ipget) != EOF) {
        semop(semid, (struct sembuf[]) { { .sem_num = 0, .sem_op = -1} }, 1);
        *vptr = ipget;
        int id;
        if ((ipget >> 31) == 0) {
            id = 1;
        } else if ((ipget >> 30) == 2) {
            id = 2;
        } else if ((ipget >> 29) == 6) {
            id = 3;
        }
        semop(semid, (struct sembuf[]) { { .sem_num = 0, .sem_op = 1} }, 1);
    }
    semctl(semid, 0, IPC_RMID);
    while (wait(NULL) > 0);
    return 0;
}
