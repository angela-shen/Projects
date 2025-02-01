#include "shell.h"

#include "gaps.h"
#include "stats.h"

#include <stdint.h>

void shell_sort(Stats *stats, uint32_t *arr, uint32_t length) {
    for (uint32_t k = 0; k < 142; k++) {
        uint32_t gap = gaps[k];
        if (gap < length) {
            for (uint32_t i = gap; i < length; i++) {
                uint32_t j = i;
                i = move(stats, i);
                uint32_t temp = arr[i];
                while (j >= gap && ((cmp(stats, temp, arr[j - gap]) < 0))) {
                    uint32_t index = move(stats, j);
                    arr[index] = arr[index - gap];
                    j -= gap;
                }
                j = move(stats, j);
                arr[j] = temp;
            }
        }
    }
}
