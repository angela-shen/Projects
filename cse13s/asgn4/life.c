#include "universe.h"

#include <inttypes.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "htsn:i:o:"

void help(char *exec) {

    printf("SYNOPSIS\n"
           "       Conway's Game of Life.\n"
           "\n"
           "USAGE\n"
           "       %s [-tsn:i:o:h]\n"
           "\n"
           "OPTIONS\n"
           "	-h		Display program help and usage.\n"
           "	-t		Create your universe as a toroidal\n"
           "	-s		Silent - do not use animate in the evolution\n"
           "	-n {number}     Number of generations [default: 100]\n"
           "	-i {file} 	Input file [default: stdin]\n"
           "	-o {file}       Output file [default: stdout]\n",
        exec);
}

int main(int argc, char **argv) {
    /* remember to delete/free/close: input/output file, char file, both universes */
    /* default length, elements and seed*/
    bool h = false;
    bool t = false;
    bool s = false;
    uint32_t gen = 100;
    FILE *input = stdin;
    FILE *output = stdout;
    /* copy file names into a char string for string copy */
    char *file = (char *) calloc(100, sizeof(char));
    /* initializes opt */
    /*step 1 parse commandline*/
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': h = true; break;
        case 't': t = true; break;
        case 's': s = true; break;
        case 'n': gen = (uint32_t) strtoul(optarg, NULL, 10); break;
        case 'i':
            strcpy(file, optarg);
            input = fopen(file, "r+");
            break;
        case 'o':
            strcpy(file, optarg);
            output = fopen(file, "w+");
            break;
        default: help(argv[0]); return 1;
        }
    }
    if (h) {
        s = false;
        t = false;
        if (input != stdin) {
            fclose(input);
        }
        if (input != stdout) {
            fclose(output);
        }
        help(argv[0]);
        return 0;
    }
    /*step 2: fscanf() for row and cols of universe*/
    /* return error if either signed ints are negative */
    int32_t x, y;
    fscanf(input, "%" SCNd32 " %" SCNd32, &x, &y);
    if (x < 0 || y < 0) {
        fprintf(stderr, "Malinformed Input\n");
        return 1;
    }
    /* step 3: create two universes with dimensions from 2 */
    x = (uint32_t) x;
    y = (uint32_t) y;
    Universe *a = uv_create(x, y, t);
    Universe *b = uv_create(x, y, t);
    /* step 4: uv populate, return error if uv populate doesn't work*/
    if (uv_populate(a, input) == false) {
        fprintf(stderr, "Malinformed Input\n");
        return 1;
    }
    /* step 5: ncurses */
    /* based on ncurses example from asgn4 spec*/
    uint32_t row = uv_rows(a);
    uint32_t col = uv_cols(a);
    uint32_t delay = 50000;
    /* if s is true, ncurses is silenced */
    if (s == false) {
        initscr();
        curs_set(FALSE);
    }
    /* step 6: ncurses + census */
    for (uint32_t i = 0; i < gen; i++) {
        /* if not silenced, display universe A */
        if (s == false) {
            clear();
            for (uint32_t r = 0; r < row; r++) {
                for (uint32_t c = 0; c < col; c++) {
                    mvprintw(r, c, (uv_get_cell(a, r, c)) ? "o" : ".");
                }
                mvprintw(r, col, "\n");
            }
            refresh();
            usleep(delay);
        }
        /* take full census of A, change B */
        for (uint32_t r = 0; r < row; r++) {
            for (uint32_t c = 0; c < col; c++) {
                /* if the cell is alive, but is overcrowded/lonely, kill it */
                if ((uv_get_cell(a, r, c))
                    && ((uv_census(a, r, c) != 2 && uv_census(a, r, c) != 3))) {
                    uv_dead_cell(b, r, c);
                }
                /* if the cell is dead and neighbors is not 3, mark it dead */
                else if (!(uv_get_cell(a, r, c)) && (uv_census(a, r, c) != 3)) {
                    uv_dead_cell(b, r, c);
                }
                /* if the cell is alive and neighbors is 3 or 2, keep it alive*/
                else if ((uv_get_cell(a, r, c))
                         && ((uv_census(a, r, c) == 2 || uv_census(a, r, c) == 3))) {
                    uv_live_cell(b, r, c);
                }
                /* if the cell is dead and neighbors is 3, revive it */
                else if (!(uv_get_cell(a, r, c)) && (uv_census(a, r, c) == 3)) {
                    uv_live_cell(b, r, c);
                }
            }
        }
        /* swap pointers */
        Universe *temp;
        temp = a;
        a = b;
        b = temp;
    }
    /* step 7 */
    if (s == false) {
        endwin();
    }
    /* step 8: output a to output file */
    uv_print(a, output);

    /* destroy things! */
    uv_delete(a);
    uv_delete(b);
    /* close file */
    if (input != stdin) {
        fclose(input);
    }
    if (output != stdout) {
        fclose(output);
    }
    free(file);
    /* return 0 for main*/
    return 0;
}
