#include <stdio.h>
#include <string.h>
#include <limits.h>

enum
{
    NUM = 10
};

int main(void)
{
    char s[PATH_MAX] = "\0";
    if (fgets(s, sizeof(s), stdin) != NULL) {
        char *end = s + strlen(s) - 1;
        while (end != s && (*end == '\n' || *end == '\r')) {
            *end = '\0';
            end--;
        }
    }
    unsigned long long cnt[NUM];
    memset(cnt, 0, sizeof(cnt));
    FILE *f = fopen(s, "r");
    if (f) {
        int c;
        while ((c = getc_unlocked(f)) != EOF) {
            if (c >= '0' && c <= '9') {
                cnt[c - '0'] += 1;
            }
        }
        fclose(f);
    }
    for (int i = 0; i < NUM; i++) {
        printf("%d %llu\n", i, cnt[i]);
    }
    return 0;
}
