#include "batcher.h"

#include "stats.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void comparator(Stats *stats, uint32_t *A, uint32_t x, uint32_t y) {
    if ((cmp(stats, A[x], A[y])) > 0) {
        swap(stats, &A[x], &A[y]);
    }
}

uint32_t bitlength(uint32_t n) {
    uint32_t x = n;
    uint32_t bits = 0;
    while (x != 0) {
        x >>= 1;
        bits++;
    }
    return bits;
}

void batcher_sort(Stats *stats, uint32_t *A, uint32_t n) {
    if (n == 0) {
        return;
    }

    uint32_t t = bitlength(n);
    uint32_t p = (1 << (t - 1));

    while (p > 0) {
        uint32_t q = (1 << (t - 1));
        uint32_t r = 0;
        uint32_t d = p;
        while (d > 0) {
            for (uint32_t i = 0; (i < (n - d)); i++) {
                if ((i & p) == r) {
                    comparator(stats, A, i, i + d);
                }
            }
            d = q - p;
            q >>= 1;
            r = p;
        }
        p >>= 1;
    }
}
