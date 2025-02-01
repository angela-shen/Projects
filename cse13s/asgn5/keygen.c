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
#include <sys/stat.h>

#define OPTIONS "b:i:n:d:s:vh"

void help(char *exec) {
    printf(
        "SYNOPSIS\n"
        "\n"
        "USAGE\n"
        "       %s [b:i:n:d:s:vh]"
        "\n"
        "OPTIONS\n"
        "	-b		Specifies the minimum bits needed for the public modulus n.\n"
        "       -i {number}	Specifies the number of Miller-Rabin iterations for testing "
        "primes. [default: 50]\n"
        "       -n {file}       Specifies the file that contains the public key [default: ss.pub]\n"
        "       -d {file}       Specifies the file that contains the private key [default: "
        "ss.priv]\n"
        "       -s {seed}	Random seed for testing.\n"
        "       -v              Enables verbose output.\n"
        "       -h              Display program help and usage.\n",
        exec);
}

int main(int argc, char **argv) {
    uint64_t b, i, s;
    b = 256;
    i = 50;
    s = time(NULL);
    bool v = false;
    bool n1 = false;
    bool d1 = false;
    char *pbfile = (char *) calloc(NAME_MAX, sizeof(char));
    char *pvfile = (char *) calloc(NAME_MAX, sizeof(char));
    //parse
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b': b = (uint64_t) strtoul(optarg, NULL, 10); break;
        case 'i': i = (uint64_t) strtoul(optarg, NULL, 10); break;
        case 'n':
            n1 = true;
            strcpy(pbfile, optarg);
            break;
        case 'd':
            d1 = true;
            strcpy(pvfile, optarg);
            break;
        case 's': s = (uint64_t) strtoul(optarg, NULL, 10); break;
        case 'v': v = true; break;
        case 'h':
            free(pbfile);
            free(pvfile);
            help(argv[0]);
            return 0;
        default:
            free(pbfile);
            free(pvfile);
            help(argv[0]);
            return 0;
        }
    }
    //fopen
    FILE *public = n1 ? fopen(pbfile, "w") : fopen("ss.pub", "w");
    FILE *private = d1 ? fopen(pvfile, "w") : fopen("ss.priv", "w");
    if (public == NULL) {
        free(pbfile);
        free(pvfile);
        printf("Failed to open public file\n");
        return 0;
    }
    if (private == NULL) {
        free(pbfile);
        free(pvfile);
        printf("Failed to open private file\n");
        return 0;
    }
    //fchmod/fileno
    fchmod(fileno(private), 0600);
    //randstate
    randstate_init(s);
    //public and private keys
    mpz_t p, q, n;
    //initialize p, q and n for make pub
    mpz_inits(p, q, n, NULL);
    ss_make_pub(p, q, n, b, i);
    mpz_t d, pq;
    //initialize d and pq for make priv
    mpz_inits(d, pq, NULL);
    ss_make_priv(d, pq, p, q);
    // based on pseudocode from username.c in resources/asgn5/section-examples/
    char *username = getenv("USER");
    //end citation
    //write files
    ss_write_pub(n, username, public);
    ss_write_priv(pq, d, private);
    //verbose
    if (v) {
        gmp_printf("user = %s\n", username);
        gmp_printf("p (%d bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_printf("q (%d bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("pq (%d bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
        gmp_printf("d (%d bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }
    //close and clear
    if (n1) {
        fclose(public);
    }
    if (d1) {
        fclose(private);
    }
    free(pbfile);
    free(pvfile);
    mpz_clears(p, q, n, d, pq, NULL);
    //clear randstate
    gmp_randclear(state);
}
