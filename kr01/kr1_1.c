#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

enum
{
    OFT1 = 4,
    OFT2 = 16,
    MASK1 = 0xF0,
    MASK2 = 0xF,
    BASE = 32
};

int main(int argc, char *argv[])
{
    uint32_t sum = 0;
    for (int i = 1; i < argc; i++) {
        sum ^= strtoul(argv[i], NULL, BASE);
    }
    uint32_t res = ((sum >> OFT1) & MASK1) | ((sum >> OFT2) & MASK2);
    printf("%" PRIx32 "\n", res);
    return 0;
}
