#include "batcher.h"
#include "heap.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "Hasbhqn:p:r:"

void help(char *exec) {
    /*implement help menu based on resources binary and monte_carlo*/
    printf("SYNOPSIS\n"
           "       A collection of comparison-based sorting algorithms.\n"
           "\n"
           "USAGE\n"
           "       %s [-Hasbhqn:p:r:] [-n length] [-p elements] [-r seed]\n"
           "\n"
           "OPTIONS\n"
           "	-H		Display program help and usage.\n"
           "	-a		Enables all sorts.\n"
           "	-s		Enable Shell Sort.\n"
           "	-b		Enable Batcher Sort.\n"
           "	-h		Enable Heap Sort.\n"
           "	-q	        Enable Quick Sort.\n"
           "	-n length       Specify number of array elements (default: 100).\n"
           "	-p elements 	Specify number of elements to print (default: 100).\n"
           "	-r seed         Specify random seed (default: 13371453).\n",
        exec);
}

/* main function */
int main(int argc, char **argv) {
    /* default length, elements and seed*/
    uint32_t length = 100;
    uint32_t elements = 100;
    uint32_t seed = 13371453;
    /* set for sort/help */
    Set arg_set = set_empty();
    uint8_t x = 0;
    /* initializes opt */
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'H':
            x = 5;
            arg_set = set_insert(arg_set, x);
            break;
        case 'a':
            for (uint8_t i = 1; i < 5; i++) {
                arg_set = set_insert(arg_set, i);
            }
            break;
        case 's':
            x = 1;
            arg_set = set_insert(arg_set, x);
            break;
        case 'b':
            x = 2;
            arg_set = set_insert(arg_set, x);
            break;
        case 'h':
            x = 3;
            arg_set = set_insert(arg_set, x);
            break;
        case 'q':
            x = 4;
            arg_set = set_insert(arg_set, x);
            break;
        case 'n': length = (uint32_t) strtoul(optarg, NULL, 10); break;
        case 'p': elements = (uint32_t) strtoul(optarg, NULL, 10); break;
        case 'r': seed = (uint32_t) strtoul(optarg, NULL, 10); break;
        }
    }
    if (set_member(arg_set, x = 5) || ((uint32_t) arg_set == 0)) {
        arg_set = set_empty();
        help(argv[0]);
    }
    /* initialize stats and pointer */
    Stats *stats = (Stats *) malloc(sizeof(Stats));
    /*make array*/
    /* code from calloc_example.c in resources */
    uint32_t *Array = (uint32_t *) calloc(length, sizeof(uint32_t));
    /* end citation */
    if (set_member(arg_set, x = 1)) {
        srandom(seed);
        /* code from calloc_example.c in resources*/
        for (uint32_t i = 0; i < length; i++) {
            Set mask = set_universal();
            mask >>= 2;
            Set bit = (uint32_t) random();
            uint32_t num = (uint32_t) (bit & mask);
            Array[i] = num;
        }
        /* end citation */
        if (length > 0) {
            shell_sort(stats, Array, length);
        }
        printf("Shell Sort, %" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64 " compares", length,
            stats->moves, stats->compares);
        for (uint32_t i = 0; ((i < elements) && (i < length)); i++) {
            if ((i % 5) == 0) {
                printf("\n");
            }
            printf("   %13" PRIu32, Array[i]);
        }
        printf("\n");
        reset(stats);
    }
    if (set_member(arg_set, x = 2)) {
        srandom(seed);
        /* code from calloc_example.c in resources*/
        for (uint32_t i = 0; i < length; i++) {
            Set mask = set_universal();
            mask >>= 2;
            Set bit = (uint32_t) random();
            uint32_t num = (uint32_t) (bit & mask);
            Array[i] = num;
        }
        /* end citation */
        if (length > 0) {
            batcher_sort(stats, Array, length);
        }
        printf("Batcher Sort, %" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64 " compares",
            length, stats->moves, stats->compares);
        for (uint32_t i = 0; ((i < elements) && (i < length)); i++) {
            if ((i % 5) == 0) {
                printf("\n");
            }
            printf("   %13" PRIu32, Array[i]);
        }
        printf("\n");
        reset(stats);
    }
    if (set_member(arg_set, x = 3)) {
        srandom(seed);
        /* code from calloc_example.c in resources*/
        for (uint32_t i = 0; i < length; i++) {
            Set mask = set_universal();
            mask >>= 2;
            Set bit = (uint32_t) random();
            uint32_t num = (uint32_t) (bit & mask);
            Array[i] = num;
        }
        /* end citation */
        if (length > 0) {
            heap_sort(stats, Array, length);
        }
        printf("Heap Sort, %" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64 " compares", length,
            stats->moves, stats->compares);
        for (uint32_t i = 0; ((i < elements) && (i < length)); i++) {
            if ((i % 5) == 0) {
                printf("\n");
            }
            printf("   %13" PRIu32, Array[i]);
        }
        printf("\n");
        reset(stats);
    }
    if (set_member(arg_set, x = 4)) {
        srandom(seed);
        /* code from calloc_example.c in resources*/
        for (uint32_t i = 0; i < length; i++) {
            Set mask = set_universal();
            mask >>= 2;
            Set bit = (uint32_t) random();
            uint32_t num = (uint32_t) (bit & mask);
            Array[i] = num;
        }
        /* end citation */
        if (length > 0) {
            quick_sort(stats, Array, length);
        }
        printf("Quick Sort, %" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64 " compares", length,
            stats->moves, stats->compares);
        for (uint32_t i = 0; ((i < elements) && (i < length)); i++) {
            if ((i % 5) == 0) {
                printf("\n");
            }
            printf("   %13" PRIu32, Array[i]);
        }
        printf("\n");
        reset(stats);
    }
    free(Array);
    return 0;
}
