#include "randstate.h"
#include "ss.h"
#include "numtheory.h"

#include <time.h>
#include <stdio.h>
#include <gmp.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define OPTIONS "i:o:n:vh"

void help(char *exec) {
    printf("SYNOPSIS\n"
           "\n"
           "USAGE\n"
           "	%s [i:o:n:vh]"
           "\n"
           "OPTIONS\n"
           "	-i {file}	Specifies which input file to decrypt [default: stdin]\n"
           "	-o {file}       Specifies which output file to decrypt [default: stdout]\n"
           "	-n {file}       Specifies the file that contains the private key [default: "
           "ss.priv]\n"
           "	-v              Enables verbose output.\n"
           "	-h		Display program help and usage.\n",
        exec);
}

int main(int argc, char **argv) {
    bool v = false;
    bool i = false;
    bool o = false;
    bool n1 = false;
    char *input = (char *) calloc(NAME_MAX, sizeof(char));
    char *output = (char *) calloc(NAME_MAX, sizeof(char));
    char *key = (char *) calloc(NAME_MAX, sizeof(char));
    //parse
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h':
            free(input);
            free(output);
            free(key);
            help(argv[0]);
            return 0;
        case 'v': v = true; break;
        case 'i':
            i = true;
            strcpy(input, optarg);
            break;
        case 'o':
            o = true;
            strcpy(output, optarg);
            break;
        case 'n':
            n1 = true;
            strcpy(key, optarg);
            break;
        default:
            free(input);
            free(output);
            free(key);
            help(argv[0]);
            return 0;
        }
    }
    //fopen
    FILE *in = i ? fopen(input, "r") : stdin;
    FILE *out = o ? fopen(output, "w") : stdout;
    FILE *privkey = n1 ? fopen(key, "r") : fopen("ss.priv", "r");
    // if any fail, free and fclose
    if (in == NULL || out == NULL || privkey == NULL) {
        free(input);
        free(output);
        free(key);
        if (in == NULL) {
            printf("Failed to open input file\n");
        }
        if (out == NULL) {
            printf("Failed to open output file\n");
        }
        (privkey == NULL) ? printf("Failed to open private key file\n") : fclose(privkey);
        return 0;
    }
    //read private key
    mpz_t pq, d;
    mpz_inits(pq, d, NULL);
    ss_read_priv(pq, d, privkey);
    //verbose output
    if (v) {
        gmp_printf("pq (%d bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
        gmp_printf("d (%d bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }
    //decrypt
    ss_decrypt_file(in, out, d, pq);
    //close files and clear variables
    if (i) {
        fclose(in);
    }
    if (o) {
        fclose(out);
    }
    fclose(privkey);
    free(input);
    free(output);
    free(key);
    mpz_clears(pq, d, NULL);
}
