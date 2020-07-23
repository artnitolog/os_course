#include <stdio.h>
#include <inttypes.h>

uint32_t to_sm(uint32_t base, uint32_t cur)
{
    if (cur >> (base - 1) == 0) {
        return cur;
    }
    uint32_t res = cur ^ (1 << (base - 1));
    return -res;
}

int main(void)
{
    uint32_t n;
    int w;
    scanf("%" PRIo32 "%o", &n, &w);
    uint32_t bnd = 1 << n;
    for (uint32_t i = 0; i != bnd; i++) {
        printf("|%*" PRIo32 "|%*" PRIu32 "|%*" PRId32 "|\n", w, i, w, i, w, to_sm(n, i));
    }
    return 0;
}