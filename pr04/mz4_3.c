#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <inttypes.h>

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

void traversal(int fd, off_t pos)
{
    if (lseek(fd, pos, SEEK_SET) == -1) {
        exit(errno);
    }
    struct Node node;
    if (read(fd, &node, sizeof(node)) != sizeof(node)) {
        exit(errno);
    }
    if (node.right_idx) {
        traversal(fd, node.right_idx * sizeof(node));
    }
    printf("%" PRId32 "\n", node.key);
    if (node.left_idx) {
        traversal(fd, node.left_idx * sizeof(node));
    }
}

int main(int argc, char **argv)
{
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        exit(errno);
    }
    traversal(fd, 0);
    printf("\n");
    close(fd);
    return 0;
}