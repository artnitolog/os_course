#include <stdio.h>
#include <stdlib.h>

void qsort_r(void *base, size_t nmemb, size_t size,
           int (*compar)(const void *, const void *, void *),
           void *arg);

struct Pair
{
    int key;
    int value;
};

int stable_cmp(const void *a, const void *b, void *data)
{
    int i = *((const int *) a);
    int j = *((const int *) b);
    struct Pair *src = (struct Pair *) data;
    if (src[i].key != src[j].key) {
        return (src[i].key > src[j].key) - (src[i].key < src[j].key);
    } else {
        return (i > j) - (i < j);
    }
}

void process(struct Pair *data, size_t size)
{
    int *order = calloc(size, sizeof(*order));
    if (!order) {
        printf(":(\n");
        exit(1);
    }
    for (int i = 0; i < size; i++) {
        order[i] = i;
    }
    qsort_r(order, size, sizeof(*order), stable_cmp, data);
    struct Pair el0 = data[0];
    int i;
    free(order);
}

int main(void)
{
    // freopen("/home/ruslan/msu/prac/oh_in", "r", stdin);
    // freopen("oh_out", "w", stdout);
    int n;
    scanf("%d", &n);
    struct Pair *a = calloc(n, sizeof(*a));
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &a[i].key, &a[i].value);
    }
    process(a, n);
    // for (int i = 0; i < n; i++) {
    //     printf("%d %d\n", a[i].key, a[i].value);
    // }
    free(a);
    return 0;
}