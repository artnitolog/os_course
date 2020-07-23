#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

enum
{
    PAR_NUM = 6,
    YEAR_OFFSET = 1900
};

int time_read(struct tm *ct, FILE *f)
{
    memset(ct, 0, sizeof(*ct));
    int ret_val = fscanf(f, "%d/%d/%d%d:%d:%d", &ct->tm_year, &ct->tm_mon,
            &ct->tm_mday, &ct->tm_hour, &ct->tm_min, &ct->tm_sec);
    ct->tm_isdst = -1;
    ct->tm_mon -= 1;
    ct->tm_year -= YEAR_OFFSET;
    return ret_val == PAR_NUM;
}

int main(int argc, char *argv[])
{
    FILE *f = fopen(argv[1], "r");
    time_t prev, cur;
    struct tm ct;
    time_read(&ct, f);
    prev = mktime(&ct);
    while (time_read(&ct, f)) {
        cur = mktime(&ct);
        printf("%ld\n", cur - prev);
        prev = cur;
    }
    fclose(f);
    return 0;
}