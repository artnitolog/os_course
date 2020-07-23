#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <string.h>
enum
{
    howmany9s = 18,
    BASE = 1000000000000000000,
    LIM_ANS = 4690,
    LIM_FRAC = 2800
};

uint32_t mul16(uint64_t *a, int n)
{
    uint32_t legacy = 0;
    for (int i = n; i >= 0; i--) {
        a[i] <<= 4; // *= 16
        a[i] += legacy;
        legacy = a[i] / BASE;
        a[i] %= BASE;
    }
    printf("%d\n", legacy);
    return legacy;
}

int main(void)
{
//    freopen("/home/ruslan/vlt.txt", "r", stdin);
//    freopen("/home/ruslan/ren.txt", "w", stdout);
    freopen("../hw/tf", "r", stdin);
    int prec2;
    scanf("%d", &prec2);
    int prec16 = (prec2 + 3) / 4; //округление в большую сторону
    static uint64_t dec_frac[LIM_FRAC];
    static uint32_t ans[LIM_ANS];
    int last = 0;
    while (scanf("%18" SCNu64, &dec_frac[last]) > 0) {
        last++;
    }
    last--; //отлично, last - заключительные цифры дробной части
    while (dec_frac[last] && dec_frac[last] < BASE) { //если вдруг нам решили передать кучку нулей
        dec_frac[last] *= 10;
    }
    dec_frac[last] /= 10;
    int dword = 0, offset = -4;
    for (int i = 0; i < prec16; i++) {
        while (dec_frac[last] == 0 && last >= 0) {
            last--;
        }
        if (last == -1) {
            if (i == 0) {
                printf("00000000\n");
                return 0;
            }
            break;
        }
        offset += 4;
        if (offset == 32) {
            dword++;
            offset = 0;
        }
        uint32_t cur = mul16(dec_frac, last);
        ans[dword] |= cur << (28 - offset);
    }
    for (int j = 0; j <= dword; j++) {
        printf("%08x ", ans[j]);
    }
    return 0;
}
