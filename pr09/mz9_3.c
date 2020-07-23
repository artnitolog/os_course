#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int perform(const char *cmd)
{
    pid_t pid = fork();
    if (!pid) {
        execlp(cmd, cmd, NULL);
        _exit(1);
    } else if (pid < 0) {
        return 0;
    }
    int status;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) && WEXITSTATUS(status) == 0;
}

int main(int argc, char *argv[])
{
    if ((perform(argv[1]) || perform(argv[2])) && perform(argv[3])) {
        return 0;
    }
    return 1;
}