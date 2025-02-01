#include "code.h"
#include "word.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

//creates a word with an array of syms and a len of len
Word *word_create(uint8_t *syms, uint32_t len) {
    Word *w = (Word *) malloc(sizeof(Word));
    w->syms = (uint8_t *) calloc(len, sizeof(uint8_t));
    w->len = len;
    for (uint32_t i = 0; i < len; i++) {
        w->syms[i] = syms[i];
    }
    return w;
}

//creates a copy of a word and appends another symbol to the end of syms
Word *word_append_sym(Word *w, uint8_t sym) {
    Word *new = (Word *) malloc(sizeof(Word));
    new->syms = (uint8_t *) calloc(w->len + 1, sizeof(uint8_t));
    new->len = (w->len + 1);
    for (uint32_t i = 0; i < w->len; i++) {
        new->syms[i] = w->syms[i];
    }
    new->syms[w->len] = sym;
    return new;
}

//deletes a word
void word_delete(Word *w) {
    if (w->len != 0) {
        free(w->syms);
    }
    free(w);
    w = NULL;
}

//creates a wordtable
WordTable *wt_create(void) {
    WordTable *wt = (WordTable *) calloc(MAX_CODE, sizeof(Word *));
    Word *empty = word_create(0, 0);
    wt[EMPTY_CODE] = empty;
    return wt;
}

//resets a wordtable to just the empty word
void wt_reset(WordTable *wt) {
    for (uint32_t i = 2; i < MAX_CODE; i++) {
        if (wt[i] == NULL) {
            continue;
        }
        word_delete(wt[i]);
        wt[i] = NULL;
    }
}

//deletes a wordtable and frees all memory
void wt_delete(WordTable *wt) {
    for (uint32_t i = 1; i < MAX_CODE; i++) {
        if (wt[i] == NULL) {
            continue;
        }
        word_delete(wt[i]);
        wt[i] = NULL;
    }
    free(wt);
    wt = NULL;
}
