#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int mysys(const char *str)
{
    pid_t pid = fork();
    if (!pid) {
        execl("/bin/sh", "sh", "-c", str, NULL);
        _exit(127);
    } else if (pid < 0) {
        return -1;
    }
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    if (WIFSIGNALED(status)) {
        return WTERMSIG(status) + 128;
    }
    return -1;
}