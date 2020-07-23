#include <inttypes.h>
#include <stdio.h>

enum
{ 
    SHIFT_BOUND = 31,
    MANTISSA = 23
};

int isfitting(uint32_t u)
{
    if (u == 0) {
        return 1;
    }
    int left_bound = SHIFT_BOUND;
    while (((u >> left_bound) & 1) == 0) {
        left_bound--;
    }
    int right_bound = 0;
    while (right_bound != left_bound && ((u >> right_bound) & 1) == 0) {
        right_bound++;
    }
    int frac_size = left_bound - right_bound;
    return frac_size <= MANTISSA;
}

int main(void)
{
    uint32_t u;
    while (scanf("%"SCNu32, &u) > 0) {
        printf("%d\n", isfitting(u));
    }
    return 0;
}