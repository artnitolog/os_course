#include <stdio.h>
#include <stdlib.h>

struct Pair
{
    int key;
    int value;
};



void merge(struct Pair *a, int l, int m, int r)
{
    int i = 0, j = 0;
    struct Pair *a_upd = calloc(r - l, sizeof(*a_upd));
    while (l + i < m && m + j < r) {
        if (a[l + i].key < a[m + j].key) {
            a_upd[i + j] = a[l + i];
            i++;
        } else {
            a_upd[i + j] = a[m + j];
            j++;
        }
    }
    for ( ; l + i < m; i++) {
        a_upd[i + j] = a[l + i];
    }
    for (int t = 0; t < i + j; t++) {
        a[l + t] = a_upd[t];
    }
    free(a_upd);
}

void merge_sort(struct Pair *a, int l, int r)
{
    if (r - l > 1) {
        int m = (l + r) / 2;
        merge_sort(a, l, m);
        merge_sort(a, m, r);
        merge(a, l, m, r);
    }
}

void process(struct Pair *data, size_t size)
{
    merge_sort(data, 0, size);
}

int main(void)
{
    freopen("/home/ruslan/msu/prac/input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int n;
    scanf("%d", &n);
    struct Pair *a = calloc(n, sizeof(*a));
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &a[i].key, &a[i].value);
    }
    merge_sort(a, 0, n);
    for (int i = 0; i < n; i++) {
        printf("%d %d\n", a[i].key, a[i].value);
    }
    free(a);
    return 0;
}