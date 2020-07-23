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
    int cur_pos = l;
    int size = m - l;
    struct Pair *buf = calloc(size, sizeof(*buf));
    if (!buf) {
        return;
    }
    for (int k = 0; k < size; k++) {
        buf[k] = a[l + k];
    }
    while (i < size && m + j < r) {
        if (buf[i].key <= a[m + j].key) {
            a[cur_pos++] = buf[i];
            i++;
        } else {
            a[cur_pos++] = a[m + j];
            j++;
        }
    }
    while (i < size) {
        a[cur_pos++] = buf[i++];
    }
    free(buf);
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
