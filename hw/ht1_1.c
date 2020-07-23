#include <stdio.h>
#include <stdlib.h>

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
