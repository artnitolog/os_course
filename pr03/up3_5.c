#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <inttypes.h>
#include <ctype.h>

// char *getline2(FILE *f);
char *getline2(FILE *f)
{
    size_t size = 1;
    int i = 0;
    int c;
    char *s = NULL;
    while ((c = getc(f)) != EOF) {
        if (i == size - 1) {
            size *= 2;
            char *new_s = realloc(s, size);
            if (!new_s) {
                free(s);
                exit(1);
            }
            s = new_s;
        }
        s[i++] = c;
        if (c == '\n') {
            break;
        }
    }
    if (i) {
        s[i] = '\0';
    }
    return s;
}

enum
{
    BASE = 10,
    EMPTY = 0x0bad1dea,
    ODD = 0xbedabeda
};

int isempty(const char *s)
{
    while (*s) {
        if (!isspace(*s++)) {
            return 0;
        }
    }
    return 1;
}

int isodd(const char *s)
{
    int has_sign = 0;
    while (*s) {
        if (isspace((unsigned char) *s)) {
            if (has_sign) {
                return 1;
            }
        } else if (isdigit((unsigned char) *s)) {
            has_sign = 0;
        } else if (*s == '-' || *s == '+') {
            if (has_sign) {
                return 1;
            }
            has_sign = 1;
        } else {
            return 1;
        }
        s++;
    }
    return 0;
}

int main(void)
{
    char *s;
    uint32_t line_num = 0;
    while ((s = getline2(stdin))) {
        line_num++;
        if (isempty(s)) {
            printf("%" PRId32 "\n", line_num + EMPTY);
        } else if (isodd(s)) {
            printf("%" PRId32 "\n", line_num + ODD);
        } else {
            int32_t sum = 0, cur = 0;
            int32_t i = 0;
            char *p = s;
            char *endptr;
            while (1) {
                i++;
                errno = 0;
                cur = strtol(p, &endptr, BASE);
                if (errno == ERANGE) {
                    if (cur > 0) {
                        cur = i;
                    } else {
                        cur = -i;
                    }
                }
                if (p == endptr) {
                    break;
                }
                p = endptr;
                __builtin_add_overflow(sum, cur, &sum);
            }
            printf("%" PRId32 "\n", sum);
        }
        free(s);
    }
    return 0;
}