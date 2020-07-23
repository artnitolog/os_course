#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n;
    scanf("%d", &n);
    int size = n - 1;
    static int matrix[1999][1999];
    for (int i = 0; i < size; i++) {
        printf("0 ");
    }
    printf("\n");
    for (int a = 1; a < n; a++) {
        int c = 0;
        for (int b = 1; b < n; b++) {
            c = (c + a) % n;
            matrix[c - 1][a - 1] = b;
        }
    }
    // for (int b = 1; b < n; b++) {
    //     int c = 0;
    //     for (int a = 1; a < n; a++) {
    //         c = (c + a) % n;
    //         matrix[c - 1][a - 1] = b;
    //     }
    // }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    return 0;
}