#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include <dirent.h>

enum
{
    YEAR_OFFSET = 1900,
    START_MDAY = 7,
    START_MON = 9,
    START_YEAR = 25,
    MIDDAY = 12,
    DATA_CNT = 3,
    CAP_MIN = 60,
    CAP_HOUR = 60,
    CAP_DAY = 24,
    CAP_MON = 30,
    CAP_YEAR = 12,
};

int main(int argc, char *argv[])
{
    struct tm ts;
    memset(&ts, 0, sizeof(ts));
    ts.tm_mday = START_MDAY;
    ts.tm_mon = START_MON;
    ts.tm_year = START_YEAR;
    ts.tm_hour = MIDDAY;
    ts.tm_isdst = -1;
    long long start_time = timegm(&ts);
    long long y, m, d;
    while (scanf("%d%d%d", &ts.tm_year, &ts.tm_mon, &ts.tm_mday) == DATA_CNT) {
        ts.tm_isdst = -1;
        ts.tm_hour = MIDDAY;
        ts.tm_year -= YEAR_OFFSET;
        ts.tm_mon -= 1;
        long long cur_time = timegm(&ts);
        unsigned long long dif = cur_time - start_time;
        memset(&ts, 0, sizeof(ts));
        dif /= CAP_MIN * CAP_HOUR * CAP_DAY;
        y = dif / (CAP_MON * CAP_YEAR);
        dif %= CAP_MON * CAP_YEAR;
        m = dif / CAP_MON;
        dif %= CAP_MON;
        d = dif;
        printf("%lld %lld %lld\n", y + 1, m + 1, d + 1);
        memset(&ts, 0, sizeof(ts));
    }
    return 0;
}