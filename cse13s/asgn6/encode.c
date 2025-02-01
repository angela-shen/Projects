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
           "	-i {file}	Specifies which input file to encode [default: stdin]\n"
           "	-o {file}       Specifies which output file to encode [default: stdout]\n"
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
    //remember to free
    char infile[NAME_MAX]; // = (char *) calloc(NAME_MAX, sizeof(char));
    char outfile[NAME_MAX]; // = (char *) calloc(NAME_MAX, sizeof(char));
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
    //get protection bit from fstat and set magic number
    struct stat s;
    fstat(infile_int, &s);
    FileHeader header = { 0 };
    header.protection = s.st_mode;
    header.magic = MAGIC;
    //open and error handle outfile
    int outfile_int = fileno(stdout);
    if (o) {
        outfile_int = open(outfile, O_CREAT | O_WRONLY, header.protection);
        if (outfile_int < 0) {
            fprintf(stderr, "Error opening outfile");
        }
    }
    //write header
    write_header(outfile_int, &header);
    //fprintf(stderr, "%d %d\n", infile_int, outfile_int);
    //create trie
    //remember to delete them all
    TrieNode *root = trie_create();
    //create curr/prev/next nodes
    TrieNode *curr_node = root;
    TrieNode *prev_node = NULL;
    TrieNode *next_node = NULL;
    uint16_t next_code = START_CODE;
    //curr/prev sym
    uint8_t curr_sym = 0;
    uint8_t prev_sym = 0;
    //while there are still symbols to read
    while (read_sym(infile_int, &curr_sym)) {
        //fprintf(stderr, "hi");
        //use trie_step for next node
        next_node = trie_step(curr_node, curr_sym);
        //if symbol has been seen, next_node is not null
        //make prev_node curr_node and curr_node next_node
        if (next_node != NULL) {
            prev_node = curr_node;
            curr_node = next_node;
        } else {
            //otherwise, write pair to outfile
            write_pair(outfile_int, curr_node->code, curr_sym, bitlength(next_code));
            //add to children of current node
            curr_node->children[curr_sym] = trie_node_create(next_code);
            //return to root
            curr_node = root;
            next_code++;
        }
        if (next_code == MAX_CODE) {
            //reset if the trie is full
            trie_reset(root);
            curr_node = root;
        }
        //make prev_sym curr_sym
        prev_sym = curr_sym;
    }
    //if current node is not the root node, then write pair for previous node
    if (curr_node->code != EMPTY_CODE && prev_node != NULL) {
        write_pair(outfile_int, prev_node->code, prev_sym, bitlength(next_code));
        next_code = (next_code + 1) % MAX_CODE;
    }
    write_pair(outfile_int, STOP_CODE, 0, bitlength(next_code));
    flush_pairs(outfile_int);
    //VERBOSE
    if (v) {
        double compress = (double) ((total_bits / 8) + 1);
        double uncompress = (double) total_syms;
        double ratio = 100 * (1 - (compress / uncompress));
        fprintf(stderr,
            "\nCompressed file size: %" PRIu64 "\nUncompressed file size: %" PRIu64
            "\nSpace Saving: %.2f%%\n",
            (total_bits / 8) + 1, total_syms, ratio);
    }
    //free/close stuff
    trie_delete(root);
    curr_node = NULL;
    prev_node = NULL;
    next_node = NULL;
    close(infile_int);
    close(outfile_int);
    return 0;
}
