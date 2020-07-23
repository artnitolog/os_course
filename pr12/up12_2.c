#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

enum
{
    BASE = 10,
    NSOPS = 2
};

void operation(int *data, int ind1, int ind2, int value);

int get_rand(int max)
{
    return rand() / (RAND_MAX + 1.0) * max;
}

void work(int semid, int *vptr, int count, int iter_count)
{
    for (int i = 0; i < iter_count; i++) {
        int ind1, ind2;
        ind1 = get_rand(count);
        ind2 = get_rand(count);
        if (ind1 == ind2) {
            continue;
        }
        struct sembuf down[] = { { ind1, -1, 0 }, { ind2, -1, 0 } };
        semop(semid, down, NSOPS);
        operation(vptr, ind1, ind2, get_rand(BASE));
        struct sembuf up[] = { { ind1, 1, 0 }, { ind2, 1, 0 } };
        semop(semid, up, NSOPS);
    }
    munmap(vptr, sizeof(*vptr) * count);
    _exit(0);
}

void err(int semid, int *vptr, int count)
{
    munmap(vptr, sizeof(*vptr) * count);
    semctl(semid, 0, IPC_RMID);
    while (wait(NULL) > 0);
    _exit(1);
}

int main(int argc, char *argv[])
{
    int count = strtol(argv[1], NULL, BASE);
    key_t key = strtol(argv[2], NULL, BASE);
    int nproc = strtol(argv[3], NULL, BASE);
    int iter_count = strtol(argv[4], NULL, BASE);
    int *vptr = mmap(NULL, sizeof(*vptr) * count, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    if (vptr == MAP_FAILED) {
        fprintf(stderr, "mmap: %s\n", strerror(errno));
        return 1;
    }
    for (int i = 0; i < count; i++) {
        scanf("%d", &vptr[i]);
    }
    int semid = semget(key, count, 0600 | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        fprintf(stderr, "semget: %s\n", strerror(errno));
        return 1;
    }
    short *values = calloc(count, sizeof(*values));
    if (!values) {
        fprintf(stderr, "calloc: %s\n", strerror(errno));
        err(semid, vptr, count);
    }
    for (int i = 0; i < count; i++) {
        values[i] = 1;
    }
    semctl(semid, 0, SETALL, values);
    free(values);
    for (int i = 0; i < nproc; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork: %s\n", strerror(errno));
            err(semid, vptr, count);
        } else if (!pid) {
            srand(strtoul(argv[i + 5], NULL, BASE));
            work(semid, vptr, count, iter_count);
        }
    }
    while (wait(NULL) > 0);
    semctl(semid, 0, IPC_RMID);
    for (int i = 0; i < count; i++) {
        printf("%d\n", vptr[i]);
        fflush(stdout);
    }
    munmap(vptr, sizeof(*vptr) * count);
    return 0;
}
