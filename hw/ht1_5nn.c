#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

enum
{
    DEC = 10,
    BITS = 32,
    TETR = 4, // tetrad
    LEN = 8, // number of digits (long arithmetic)
    BASE = 100000000, // base for long arithmetic
    LIM_ANS = 4688, // 4688 = 150000 / 32
    LIM_FRAC = 6250, // 6250 = 50000 / 8
    MID = 50000000 // case 0.5
};

uint32_t mul(uint32_t *a, int n, uint32_t shift)
{
    uint32_t legacy = 0;
    for (int i = n; i >= 0; i--) {
        a[i] <<= shift;
        a[i] += legacy;
        legacy = a[i] / BASE;
        a[i] %= BASE;
    }
    return legacy;
}

int check(uint32_t *a, int n)
{
    if (a[0] < MID) {
        return -1;
    }
    if (a[0] == MID && n == 0) {
        return 0;
    }
    return 1;
}

int rounding(uint32_t *ans, int n, int k, int rest)
{
    if (rest == 1 || (rest == 0 && ((ans[n] >> k) & 1))) {
        ans[n] += 1 << k;
        while (ans[n] == 0 && n > 0) {
            n--;
            ans[n] += 1;
        }
        if (ans[n] == 0) {
            return 1;
        }
    }
    return 0;
}



int main(void)
{
    int prec2;
    scanf("%d", &prec2);
    int prec16 = prec2 / TETR;
    static uint32_t dec_frac[LIM_FRAC];
    static uint32_t ans[LIM_ANS];
    int last = -1, cnt = 0;
    char buf[LEN + 1];
    while (scanf("%8s", buf) > 0) {
        cnt = strlen(buf);
        sscanf(buf, "%8" SCNu32, &dec_frac[++last]);
    }
    for (int i = LEN - cnt; i > 0; i--) {
        dec_frac[last] *= DEC;
    }
    int dword = 0, offset = BITS;
    for (int i = 0; i < prec16; i++) {
        while (dec_frac[last] == 0 && last > 0) {
            last--;
        }
        if (last == 0 && dec_frac[last] == 0) {
            break;
        }
        offset -= TETR;
        if (offset == -TETR) {
            dword++;
            offset = BITS - TETR;
        }
        ans[dword] |= mul(dec_frac, last, TETR) << offset;
    }
    int alpha = prec2 % TETR;
    for (int i = 0; i < alpha; i++) {
        while (dec_frac[last] == 0 && last > 0) {
            last--;
        }
        if (last == 0 && dec_frac[last] == 0) {
            break;
        }
        offset--;
        if (offset == -1) {
            dword++;
            offset = BITS - 1;
        }
        ans[dword] |= mul(dec_frac, last, 1) << offset;
    }
    while (dec_frac[last] == 0 && last > 0) {
        last--;
    }
    int rest = check(dec_frac, last);
    if (rounding(ans, dword, offset, rest)) {
        printf("1 00000000\n");
    } else {
        for (int j = 0; j < dword; j++) {
            printf("%08x ", ans[j]);
        }
        printf("%08x\n", ans[dword]);
    }
    return 0;
}