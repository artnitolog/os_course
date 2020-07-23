#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    int nproc = strtol(argv[1], NULL, 10);
    key_t key = strtol(argv[2], NULL, 10);
    int32_t maxval = strtol(argv[3], NULL, 10);
    int semid = semget(key, nproc, 0600 | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        fprintf(stderr, "semget: %s\n", strerror(errno));
        _exit(1);
    }
    semctl(semid, 0, SETVAL, 1);
    int32_t *vptr = mmap(NULL, sizeof(*vptr), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    *vptr = 0;
    int *who = mmap(NULL, sizeof(*who), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    *who = -1;
    for (int i = 0; i < nproc; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            munmap(vptr, sizeof(*vptr));
            munmap(who, sizeof(*who));
            semctl(semid, 0, IPC_RMID);
            break;
        } else if (!pid) {
            struct sembuf down[] = { { .sem_num = i, .sem_op = -1 } };
            while (semop(semid, down, 1) != -1) {
                printf("%d %" PRId32 " %d\n", i + 1, (*vptr)++, *who + 1);
                fflush(stdout);
                if (*vptr > maxval) {
                    semctl(semid, 0, IPC_RMID);
                    break;
                }
                *who = i;
                int64_t where = 1;
                for (int j = 0; j < 4; j++) {
                    where *= *vptr;
                    where %= nproc;
                }
                struct sembuf up[] = { { .sem_num = where, .sem_op = 1 } };
                semop(semid, up, 1);
            }
            munmap(vptr, sizeof(*vptr));
            munmap(who, sizeof(*who));
            _exit(0);
        }
    }
    while (wait(NULL) > 0);
    munmap(vptr, sizeof(*vptr));
    munmap(who, sizeof(*who));
    return 0;
}