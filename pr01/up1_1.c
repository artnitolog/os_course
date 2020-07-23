#include <stdio.h>
#include <limits.h>

int main(void)
{
    int a, b;
    scanf("%d %d", &a, &b);
    int c_left = INT_MAX, c_right = INT_MIN;
    if (a >= 0) {
        c_left = a - a % b;
        if (b <= INT_MAX - c_left) {
            c_right = c_left + b;
        } else {
            c_right = c_left;
        }
    }
    else {
        c_right = a - a % b;
        if (b + INT_MIN <= c_right) {
            c_left = c_right - b;
        } else {
            c_left = c_right;
        }
    }
    int c;
    if (c_right == INT_MIN) {
        c = c_left;
    } else if (c_left == INT_MAX) {
        c = c_right;
    } else if (c_right - a <= a - c_left) {
        c = c_right;
    } else {
        c = c_left;
    }
    printf("%d\n", c);
    return 0;
}
