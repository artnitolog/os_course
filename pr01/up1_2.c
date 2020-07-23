#include <stdio.h>

int main(void)
{
    int c;
    int cur_len = 0, max_len = 0;
    int has_q = 0;
    do {
        c = getchar();
        if (c >= 'a' && c <= 'z') {
            cur_len++;
            if (c == 'q') {
                has_q = 1;
            }
        }
        else {
            if (!has_q && cur_len > max_len) {
                max_len = cur_len;
            }
            cur_len = 0;
            has_q = 0;
        }
    } while (c != EOF);
    printf("%d\n", max_len);
    return 0;
}
