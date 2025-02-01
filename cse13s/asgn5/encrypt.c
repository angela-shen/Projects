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
#include <errno.h>

#define OPTIONS "i:o:n:vh"

void help(char *exec) {
    printf(
        "SYNOPSIS\n"
        "\n"
        "USAGE\n"
        "       %s [i:o:n:vh]"
        "\n"
        "OPTIONS\n"
        "       -i {file}       Specifies which input file to encrypt [default: stdin]\n"
        "       -o {file}       Specifies which output file to encrypt [default: stdout]\n"
        "       -n {file}       Specifies the file that contains the public key [default: ss.pub]\n"
        "       -v              Enables verbose output.\n"
        "       -h              Display program help and usage.\n",
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
        }
    }
    //fopen
    FILE *in = i ? fopen(input, "r") : stdin;
    FILE *out = o ? fopen(output, "w") : stdout;
    FILE *pubkey = n1 ? fopen(key, "r") : fopen("ss.pub", "r");
    // if any fail, free and fclose
    if (in == NULL || out == NULL || pubkey == NULL) {
        free(input);
        free(output);
        free(key);
        if (in == NULL) {
            printf("Failed to open input file\n");
        }
        if (out == NULL) {
            printf("Failed to open output file\n");
        }
        (pubkey == NULL) ? printf("Failed to open public key file\n") : fclose(pubkey);
        return 0;
    }
    //read public key
    char *username = (char *) calloc(LOGIN_NAME_MAX, sizeof(char));
    mpz_t n;
    mpz_init(n);
    ss_read_pub(n, username, pubkey);
    //verbose output
    if (v) {
        gmp_printf("user = %s\n", username);
        gmp_printf("n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
    }
    //encrypt
    printf("%d %d\n", in == stdin, out == stdout);
    ss_encrypt_file(in, out, n);
    //close files and clear variables
    if (i) {
        fclose(in);
    }
    if (o) {
        fclose(out);
    }
    fclose(pubkey);
    free(input);
    free(output);
    free(key);
    free(username);
    mpz_clear(n);
}
