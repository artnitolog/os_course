#include <inttypes.h>

int32_t satsum(int32_t v1, int32_t v2)
{
    int32_t ans;
    if (__builtin_add_overflow(v1, v2, &ans)) {
        int32_t max = (uint32_t) ~0 >> 1;
        if (v1 > 0) {
            return max;
        } else {
            return ~max;
        }
    } else {
        return ans;
    }
}
