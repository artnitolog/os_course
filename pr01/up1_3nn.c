#include <stdio.h>
#include <stdlib.h>

struct Pair
{
    int key;
    int value;
};

typedef struct MyPair
{
    int key;
    int value;
    int index;
} MyPair;

int stable_cmp(const void *a, const void *b)
{
    const MyPair *pa = (const MyPair *) a;
    const MyPair *pb = (const MyPair *) b;
    if (pa->key != pb->key) {
        return (pa->key > pb->key) - (pa->key < pb->key);
    } else {
        return (pa->index > pb->index) - (pa->index < pb->index);
    }
}

void process(struct Pair *data, size_t size)
{
    MyPair *my_data = calloc(size, sizeof(*my_data));
    for (int i = 0; i < size; i++) {
        my_data[i].key = data[i].key;
        my_data[i].value = data[i].value;
        my_data[i].index = i;
    }
    qsort(my_data, size, sizeof(*my_data), stable_cmp);
    for (int i = 0; i < size; i++) {
        data[i].key = my_data[i].key;
        data[i].value = my_data[i].value;
    }
    free(my_data);
}

int main(void)
{
    freopen("/home/ruslan/msu/prac/mil_in", "r", stdin);
    freopen("qsort", "w", stdout);
    int n;
    scanf("%d", &n);
    struct Pair *a = calloc(n, sizeof(*a));
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &a[i].key, &a[i].value);
    }
    // merge_sort(a, 0, n);
    process(a, n);
    for (int i = 0; i < n; i++) {
        printf("%d %d\n", a[i].key, a[i].value);
    }
    free(a);
    return 0;
}