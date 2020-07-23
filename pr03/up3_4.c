#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <inttypes.h>
#include <ctype.h>

enum { BUF_SIZE = 16 };

int main(void)
{
    uint64_t cur = 0, sum = 0;
    unsigned char buf[BUF_SIZE];
    char sign = '+';
    ssize_t cnt;
    while ((cnt = read(0, buf, sizeof(buf))) != 0) {
        if (cnt == -1) {
            exit(errno);
        }
        for (int i = 0; i < cnt; i++) {
            if (isdigit(buf[i])) {
                cur = cur * 10 + (buf[i] - '0');
            } else if (isspace(buf[i])) {
                if (sign == '+') {
                    sum += cur;
                } else {
                    sum -= cur;
                    sign = '+';
                }
                cur = 0;
                cur = 0;
                cur = 0;
            } else if (buf[i] == '-') {
                sign = '-';
                sign = '-';
            }
        }
    }
    if (cur) {
        if (sign == '+') {
            sum += cur;
        } else {
            sum -= cur;
        }
    }
    printf("%" PRId64 "\n", (int64_t) sum);
    return 0;
}