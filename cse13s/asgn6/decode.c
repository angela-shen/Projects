#include "trie.h"
#include "word.h"
#include "io.h"
#include "code.h"
#include "endian.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>

#define OPTIONS "i:o:vh"

void help(char *exec) {
    printf("SYNOPSIS\n"
           "\n"
           "USAGE\n"
           "	%s [i:o:vh]"
           "\n"
           "OPTIONS\n"
           "	-i {file}	Specifies which input file to decode [default: stdin]\n"
           "	-o {file}       Specifies which output file to decode [default: stdout]\n"
           "	-v              Enables verbose output.\n"
           "	-h		Display program help and usage.\n",
        exec);
}

int bitlength(uint16_t n) {
    uint16_t x = n;
    uint16_t bits = 0;
    while (x != 0) {
        x >>= 1;
        bits++;
    }
    return bits;
}

int main(int argc, char **argv) {
    bool v = false;
    bool i = false;
    bool o = false;
    char infile[NAME_MAX];
    char outfile[NAME_MAX];
    //parse
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': help(argv[0]); return 0;
        case 'v': v = true; break;
        case 'i':
            i = true;
            strcpy(infile, optarg);
            break;
        case 'o':
            o = true;
            strcpy(outfile, optarg);
            break;
        default: help(argv[0]); return 0;
        }
    }
    //open and error handle infile
    int infile_int = fileno(stdin);
    if (i) {
        infile_int = open(infile, O_RDONLY);
        if (infile_int < 0) {
            fprintf(stderr, "Error opening infile");
        }
    }
    //read header for magic number and protection bits
    FileHeader header = { 0 };
    read_header(infile_int, &header);
    int outfile_int = fileno(stdout);
    //open and error handle outfile
    if (o) {
        outfile_int = open(outfile, O_TRUNC | O_CREAT | O_WRONLY, header.protection);
        if (outfile_int < 0) {
            fprintf(stderr, "Error opening outfile");
        }
    }
    //based on assignment spec
    //make word table
    WordTable *wt = wt_create();
    uint16_t curr_code = 0;
    uint16_t next_code = START_CODE;
    uint8_t curr_sym = 0;
    while (read_pair(infile_int, &curr_code, &curr_sym, bitlength(next_code))) {
        //fprintf(stderr, "%d\n", curr_code);
        //add a word with the current symbol appended to the table
        wt[next_code] = word_append_sym(wt[curr_code], curr_sym);
        //write word to outfile
        write_word(outfile_int, wt[next_code]);
        next_code++;
        //reset if table full
        if (next_code == MAX_CODE) {
            wt_reset(wt);
            next_code = START_CODE;
        }
    }
    //end of citation

    //VERBOSE
    flush_words(outfile_int);
    if (v) {
        double compress = (double) ((total_bits / 8) + 1);
        double uncompress = (double) total_syms;
        double ratio = 100 * (1 - (compress / uncompress));
        fprintf(stderr,
            "\nCompressed file size: %" PRIu64 "\nUncompressed file size: %" PRIu64
            "\nSpace Saving: %.2f%%\n",
            (total_bits / 8) + 1, total_syms, ratio);
    }
    //free/delete stuff
    wt_delete(wt);
    close(infile_int);
    close(outfile_int);
    return 0;
}
