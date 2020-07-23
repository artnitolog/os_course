#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

enum
{
    LIM_ANS = 4690,
    LIM_STR = 50000
};

uint32_t mul2(char *beg, char *end)
{
    uint32_t cur = 0;
    while (end > beg) {
        cur += (*end - '0') * 2;
        *end = '0' + cur % 10;
        cur /= 10;
        end--;
    }
    cur += (*beg - '0') * 2;
    *beg = '0' + cur % 10;
    cur /= 10;
    return cur;
}

int check_rem(char *beg, char *end) 
{
    if (*beg > '5') {
        return 1;
    } else if (*beg < '5') {
        return -1;
    } else if (beg == end) {
        return 0;
    } else {
        return 1;
    }
}

int rounding(uint32_t *ans, int dword, int offset, int test)
{
    if (test == 1 || (test == 0 && ((ans[dword] >> (31 - offset)) & 1))) {
        ans[dword] += 1 << (31 - offset);
        while (ans[dword--] == 0 && dword >= 0) {
            ans[dword] += 1;
        }
        if (dword == -1 && ans[0] == 0) {
            return 1;
        }
    }
    return 0;
}

int main(void)
{
    freopen("/home/ruslan/vlt.txt", "r", stdin);
    freopen("/home/ruslan/tmptmp.txt", "w", stdout);
//    freopen("../hw/tf", "r", stdin);
    int prec;
    scanf("%d", &prec);
    static uint32_t ans[LIM_ANS];
    static char beg[LIM_STR + 2];
    scanf("%50001s", beg);
    char *end = beg + strlen(beg) - 1;
    int dword = 0, offset = -1;
    int i;
    for (i = 0; i < prec; i++) {
        while (*end == '0' && end > beg) {
            end--;
        }
        if (*end == '0' && end == beg) {
            break;
        }
        if (++offset == 32) {
            dword++;
            offset = 0;
        }
        ans[dword] |= mul2(beg, end) << (31 - offset);
    }
//    return 0;
    if (i == 0) {
        printf("00000000\n");
        return 0;
    }
    while (*end == '0' && end > beg) {
        end--;
    }
    int test = check_rem(beg, end);
    if (offset < 0 || rounding(ans, dword, offset, test)) {
        printf("1 00000000\n");
    } else {
        while (dword && !ans[dword]) {
            dword--;
        }
        for (int j = 0; j < dword; j++) {
            printf("%08x ", ans[j]);
        }
        printf("%08x\n", ans[dword]);
    }
    return 0;
}
