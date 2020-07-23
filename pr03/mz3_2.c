//commit: add scanf check


#include <stdio.h>

int main(int argc, char **argv)
{
    long long pos = 0, neg = 0;
    int cur;
    for (int i = 1; i < argc; i++) {
        if (sscanf(argv[i], "%d", &cur) != 1) {
            break;
        }
        if (cur > 0) {
            pos += cur;
        } else {
            neg += cur;
        }
    }
    printf("%lld\n%lld\n", pos, neg);
    return 0;
}