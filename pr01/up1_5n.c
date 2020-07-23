#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum { LIM = 2000 };

int is_prime(int n)
{
    if (n <= 1 || (n != 2 && n % 2 == 0)) {
        return 0;
    }
    int bound = sqrt(n);
    for (int i = 3; i <= bound; i += 2) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main(void)
{
    int n;
    scanf("%d", &n);
    if (!is_prime(n) || n >= LIM) {
        return 1;
    }
    int *a = calloc(n, sizeof(*a));
    if (!a) {
        return 1;
    }
    a[1] = 1;
    for (int i = 2; i < n; i++) {
        a[i] = n - n / i * a[n % i] % n;
    }
    for (int c = 0; c < n; c++) {
        for (int i = 1; i < n; i++) {
            printf("%d ", c * a[i] % n);
        }
        printf("\n");
    }
    free(a);
    return 0;
}