#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

enum
{
    USR_SHIFT = 6,
    GRP_SHIFT = 3
};

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};

static int eq_uid(uid_t file_uid, unsigned task_uid) {
    return file_uid == task_uid;
}

static int eq_gid(gid_t file_gid, int gid_count, unsigned *gid_list)
{
    for (int i = 0; i < gid_count; i++) {
        if (file_gid == gid_list[i]) {
            return 1;
        }
    }
    return 0;
}

static int overlay(int access, mode_t mask)
{
    return (access & mask) == access;
}

int myaccess(const struct stat *stb, const struct Task *task, int access)
{
    if (task->uid == 0)
        return 1;
    if (eq_uid(stb->st_uid, task->uid)) {
        return overlay(access, stb->st_mode >> USR_SHIFT);
    }
    if (eq_gid(stb->st_gid, task->gid_count, task->gids)) {
        return overlay(access, stb->st_mode >> GRP_SHIFT);
    }
    return overlay(access, stb->st_mode);
}
