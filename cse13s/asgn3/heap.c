#include "heap.h"

#include "stats.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

uint32_t max_child(Stats *stats, uint32_t *A, uint32_t first, uint32_t n) {
    uint32_t left = 2 * first;
    uint32_t right = left + 1;
    if (right <= n && (cmp(stats, A[right - 1], A[left - 1]) > 0)) {
        return right;
    }
    return left;
}

void fix_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    int found = 0;
    uint32_t node = first;
    uint32_t great = max_child(stats, A, node, last);
    while ((node <= (last / 2)) && (found == 0)) {
        if ((cmp(stats, A[node - 1], A[great - 1])) < 0) {
            swap(stats, &A[node - 1], &A[great - 1]);
            node = great;
            great = max_child(stats, A, node, last);
        } else {
            found = 1;
        }
    }
}

void build_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t n) {
    for (uint32_t i = (n / 2); i != (first - 1); i--) {
        fix_heap(stats, A, i, n);
    }
}

void heap_sort(Stats *stats, uint32_t *A, uint32_t n) {
    uint32_t first = 1;
    build_heap(stats, A, first, n);
    for (uint32_t i = n; i != first; i--) {
        swap(stats, &A[i - 1], &A[first - 1]);
        fix_heap(stats, A, first, i - 1);
    }
}
