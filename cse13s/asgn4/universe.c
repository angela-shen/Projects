#include "universe.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

/*returns universe pointer, malloc universe pointer*/
Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *uni = (Universe *) malloc(sizeof(Universe));
    /* based on code provided in asgn4 spec, section 3 pg 3*/
    uni->grid = (bool **) calloc(rows, sizeof(bool *));
    for (uint32_t i = 0; i < rows; i++) {
        uni->grid[i] = (bool *) calloc(cols, sizeof(bool));
    }
    /* end citation */
    uni->rows = rows;
    uni->cols = cols;
    uni->toroidal = toroidal;
    return uni;
}

/* free items in grid, then grid, then universe */
void uv_delete(Universe *u) {
    for (uint32_t i = 0; i < u->rows; i++) {
        free(u->grid[i]);
    }
    free(u->grid);
    free(u);
}

uint32_t uv_rows(Universe *u) {
    return u->rows;
}

uint32_t uv_cols(Universe *u) {
    return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r < u->rows || c < u->cols) {
        u->grid[r][c] = true;
    }
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r < u->rows || c < u->cols) {
        u->grid[r][c] = false;
    }
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r < u->rows && c < u->cols) {
        return u->grid[r][c];
    } else {
        return false;
    }
}

/* use fscanf to read from infile */
bool uv_populate(Universe *u, FILE *infile) {
    bool scan = true;
    int32_t scan_num = 0;
    int32_t x, y;
    while (feof(infile) == 0) {
        scan_num = fscanf(infile, "%" SCNd32 " %" SCNd32, &x, &y);
        /* input is INVALID if: */
        /* invalid number of entries */
        /* x or y is greater than the # of rows/cols */
        /* x or y is negative */
        if ((scan_num != 2 && scan_num != -1) || x >= ((int32_t) (u->rows))
            || y >= ((int32_t) (u->cols)) || x < 0 || y < 0) {
            scan = false;
            break;
        } else {
            uv_live_cell(u, (uint32_t) x, (uint32_t) y);
        }
    }
    return scan;
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t row = u->rows;
    uint32_t col = u->cols;
    bool toroidal = u->toroidal;
    uint32_t neighbors = 0;

    /* top left diagonal */
    /* does not exist if non-toroidal r - 1 is not valid or c - 1 is not valid */
    if ((r - 1 < row && c - 1 < col) || toroidal) {
        /* r, c is top left corner and toroidal */
        if (r - 1 >= row && c - 1 >= col && toroidal) {
            neighbors += (u->grid[row - 1][col - 1]) ? 1 : 0;
        }
        /* r, c is at top row and toroidal */
        else if (r - 1 >= row && c - 1 < col && toroidal) {
            neighbors += (u->grid[row - 1][c - 1]) ? 1 : 0;
        }
        /* r, c is at left column and toroidal */
        else if (r - 1 < row && c - 1 >= col && toroidal) {
            neighbors += (u->grid[r - 1][col - 1]) ? 1 : 0;
        }
        /* top left diagonal is a point on the grid*/
        else if (r - 1 < row && c - 1 < col) {
            neighbors += (u->grid[r - 1][c - 1]) ? 1 : 0;
        }
    }
    /* square above r, c */
    /* does not exist if non-toroidal r - 1 is not valid */
    if ((r - 1 < row) || toroidal) {
        /* r, c is at top row and toroidal */
        if (r - 1 >= row && toroidal) {
            neighbors += (u->grid[row - 1][c]) ? 1 : 0;
        }
        /* square above r, c is a point on the grid*/
        else if (r - 1 < row) {
            neighbors += (u->grid[r - 1][c]) ? 1 : 0;
        }
    }
    /* top right diagonal */
    /* does not exist if non-toroidal, r - 1 is not valid or c + 1 is not valid */
    if ((r - 1 < row && c + 1 < col) || toroidal) {
        /* r, c is top right corner and toroidal */
        if (r - 1 >= row && c + 1 >= col && toroidal) {
            neighbors += (u->grid[row - 1][0]) ? 1 : 0;
        }
        /* r, c is at top row and toroidal */
        else if (r - 1 >= row && c + 1 < col && toroidal) {
            neighbors += (u->grid[row - 1][c + 1]) ? 1 : 0;
        }
        /* r, c is at left column and toroidal */
        else if (r - 1 < row && c + 1 >= col && toroidal) {
            neighbors += (u->grid[r - 1][0]) ? 1 : 0;
        }
        /* top right diagonal is a point on the grid*/
        else if (r - 1 < row && c + 1 < col) {
            neighbors += (u->grid[r - 1][c + 1]) ? 1 : 0;
        }
    }
    /* square to the left of r, c */
    /* does not exist if non-toroidal, c - 1 is not valid */
    if ((c - 1 < col) || toroidal) {
        /* r, c is at left column and toroidal */
        if (c - 1 >= col && toroidal) {
            neighbors += (u->grid[r][col - 1]) ? 1 : 0;
        }
        /* square to the left of r, c is a point on the grid*/
        else if (c - 1 < col) {
            neighbors += (u->grid[r][c - 1]) ? 1 : 0;
        }
    }
    /* square to the right of r, c */
    /* does not exist if non-toroidal, c + 1 is not valid */
    if ((c + 1 < col) || toroidal) {
        /* r, c is at right column and toroidal */
        if (c + 1 >= col && toroidal) {
            neighbors += (u->grid[r][0]) ? 1 : 0;
        }
        /* square to the left of r, c is a point on the grid*/
        else if (c + 1 < col) {
            neighbors += (u->grid[r][c + 1]) ? 1 : 0;
        }
    }
    /* bottom left diagonal */
    /* does not exist if non-toroidal r + 1 is not valid or c - 1 is not valid */
    if ((r + 1 < row && c - 1 < col) || toroidal) {
        /* r, c is bottom left corner and toroidal */
        if (r + 1 >= row && c - 1 >= col && toroidal) {
            neighbors += (u->grid[0][col - 1]) ? 1 : 0;
        }
        /* r, c is at bottom row and toroidal */
        else if (r + 1 >= row && c - 1 < col && toroidal) {
            neighbors += (u->grid[0][c - 1]) ? 1 : 0;
        }
        /* r, c is at left column and toroidal */
        else if (r + 1 < row && c - 1 >= col && toroidal) {
            neighbors += (u->grid[r + 1][col - 1]) ? 1 : 0;
        }
        /* bottom left diagonal is a point on the grid*/
        else if (r + 1 < row && c - 1 < col) {
            neighbors += (u->grid[r + 1][c - 1]) ? 1 : 0;
        }
    }
    /* square below r, c */
    /* does not exist if non-toroidal, r + 1 is not valid */
    if ((r + 1 < row) || toroidal) {
        /* r, c is at bottom row and toroidal */
        if (r + 1 >= row && toroidal) {
            neighbors += (u->grid[0][c]) ? 1 : 0;
        }
        /* square to the left of r, c is a point on the grid*/
        else if (r + 1 < row) {
            neighbors += (u->grid[r + 1][c]) ? 1 : 0;
        }
    }
    /* bottom right diagonal */
    /* does not exist if non-toroidal r + 1 is not valid or c + 1 is not valid */
    if ((r + 1 < row && c + 1 < col) || toroidal) {
        /* r, c is bottom right corner and toroidal */
        if (r + 1 >= row && c + 1 >= col && toroidal) {
            neighbors += (u->grid[0][0]) ? 1 : 0;
        }
        /* r, c is at bottom row and toroidal */
        else if (r + 1 >= row && c + 1 < col && toroidal) {
            neighbors += (u->grid[0][c + 1]) ? 1 : 0;
        }
        /* r, c is at right column and toroidal */
        else if (r + 1 < row && c + 1 >= col && toroidal) {
            neighbors += (u->grid[r + 1][0]) ? 1 : 0;
        }
        /* bottom left diagonal is a point on the grid*/
        else if (r + 1 < row && c + 1 < col) {
            neighbors += (u->grid[r + 1][c + 1]) ? 1 : 0;
        }
    }
    return neighbors;
}

void uv_print(Universe *u, FILE *outfile) {
    uint32_t row = u->rows;
    uint32_t col = u->cols;
    int chr;
    for (uint32_t r = 0; r < row; r++) {
        for (uint32_t c = 0; c < col; c++) {
            /* print o if true, . if false */
            chr = (u->grid[r][c]) ? (int) 'o' : (int) '.';
            fputc(chr, outfile);
        }
        fputc((int) '\n', outfile);
    }
}
