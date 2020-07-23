#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

enum
{
    BASE = 10,
};

struct Msgbuf
{
    long mtype;
    uint64_t x[2];
};

int main(int argc, char *argv[])
{
    key_t key = strtol(argv[1], NULL, BASE);
    int n = strtol(argv[2], NULL, BASE);
    uint64_t val1 = strtoull(argv[3], NULL, BASE);
    uint64_t val2 = strtoull(argv[4], NULL, BASE);
    uint64_t maxval = strtoull(argv[5], NULL, BASE);
    int msgid = msgget(key, 0600 | IPC_CREAT | IPC_EXCL);
    if (msgid < 0) {
        fprintf(stderr, "msgget: %s\n", strerror(errno));
        _exit(1);
    }
    for (int i = 1; i <= n; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork: %s\n", strerror(errno));
            msgctl(msgid, IPC_RMID, NULL); // no need in kill, because
            while (wait(NULL) > 0); // removed queue will break all the loops
            _exit(1);
        } else if (!pid) {
            struct Msgbuf sms;
            while (msgrcv(msgid, &sms, sizeof(sms.x), i, 0) == sizeof(sms.x)) {
                uint64_t x3 = sms.x[0] + sms.x[1];
                printf("%d %" PRIu64 "\n", i - 1, x3);
                fflush(stdout);
                if (x3 > maxval) {
                    msgctl(msgid, IPC_RMID, NULL);
                    break;
                }
                sms = (struct Msgbuf) { .mtype = x3 % n + 1, .x = { sms.x[1], x3 } };
                if (msgsnd(msgid, &sms, sizeof(sms.x), 0) < 0) {
                    fprintf(stderr, "msgsnd: %s\n", strerror(errno));
                    msgctl(msgid, IPC_RMID, NULL);
                    break;
                }
            }
            return 0;
        }
    }
    struct Msgbuf init_sms = { .mtype = 1, .x = {val1, val2} };
    msgsnd(msgid, &init_sms, sizeof(init_sms.x), 0);
    while (wait(NULL) > 0);
    return 0;
}