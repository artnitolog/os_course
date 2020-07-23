#include <ctype.h>

int mystrspccmp(const char *str1, const char *str2)
{
    while (isspace(*str1)) {
        str1++;
    }
    while (isspace(*str2)) {
        str2++;
    }
    while (*str1 && *str1 == *str2) {
        str1++;
        str2++;
        while (isspace(*str1)) {
            str1++;
        }
        while (isspace(*str2)) {
            str2++;
        }
    }
    return (unsigned char) *str1 - (unsigned char) *str2;
}