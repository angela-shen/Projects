#include "numtheory.h"
#include "randstate.h"
#include "ss.h"

#include <stdio.h>
#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {
    uint64_t pbits, qbits;
    //auxiliary variable for calculating divisibility
    //of p and q - 1, q and p - 1
    mpz_t minus_one;
    mpz_init(minus_one);

    //generate a random number such that pbits is between nbits / 5 and 2 * nbits / 5
    //auxiliary variables for nbits / 5 and pbits
    mpz_t bits, bit;
    mpz_init(bit);
    mpz_init_set_ui(bits, nbits);
    mpz_fdiv_q_ui(bits, bits, 5); // nbits / 5
    mpz_urandomm(bit, state, bits); //generate number between 0 and nbit / 5
    //add nbits / 5 to the random number
    //number will be betwee nbits / 5 and 2 * nbits / 5
    mpz_add(bit, bit, bits);
    pbits = mpz_get_ui(bit);
    while (true) {
        //make a prime p
        make_prime(p, pbits, iters);
        //calculate qbits
        //find the number of bits in p ^ 2
        mpz_mul(bit, p, p);
        //subtract the bits of p ^ 2 from nbits
        qbits = nbits - mpz_sizeinbase(bit, 2);
        //make a prime q
        make_prime(q, qbits, iters);
        //check for divisibility
        mpz_sub_ui(minus_one, q, 1);
        if (mpz_divisible_p(minus_one, p) == 0) {
            mpz_sub_ui(minus_one, p, 1);
            if (mpz_divisible_p(minus_one, q) == 0) {
                break;
            }
        }
    }
    //n = pq
    mpz_mul(n, p, q);
    //n = p^2 * q
    mpz_mul(n, n, p);
    //clear mpz variables initialized
    mpz_clears(minus_one, bit, bits, NULL);
}

void ss_make_priv(mpz_t d, mpz_t pq, const mpz_t p, const mpz_t q) {
    mpz_t p_minus, q_minus, num, denom, lambda, n;
    mpz_inits(p_minus, q_minus, num, denom, lambda, n, NULL);
    mpz_sub_ui(p_minus, p, 1); //p - 1
    mpz_sub_ui(q_minus, q, 1); //q - 1
    mpz_mul(num, p_minus, q_minus); //(p - 1) (q - 1)
    gcd(denom, p_minus, q_minus); //gcd(p - 1, q - 1)
    mpz_divexact(lambda, num, denom); //calculate lambda pq
    mpz_mul(n, p, p);
    mpz_mul(n, n, q); //calculate n = p * p * q
    mod_inverse(d, n, lambda); //calculate mod inverse of n % lambda to derive d
    mpz_mul(pq, p, q); // pq = p * q
    mpz_clears(p_minus, q_minus, num, denom, lambda, n, NULL); // clear
}

void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n%s\n", n, username);
}

void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n%Zx\n", pq, d);
}

void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx", n);
    gmp_fscanf(pbfile, "%s", username);
}

void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx", pq);
    gmp_fscanf(pvfile, "%Zx", d);
}

void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n) {
    pow_mod(c, m, n, n);
}

void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n) {
    //calculate k = (log(sqrt(n)) - 1) / 8
    size_t k = ((uint64_t) ((1.0 / 2.0) * mpz_sizeinbase(n, 2) - 1)) / 8;
    assert(k > 1);
    //make an array of k bytes
    uint8_t *bytes = (uint8_t *) calloc(k, sizeof(uint8_t));
    bytes[0] = 0xFF;
    size_t j = 0;
    mpz_t m, print;
    //initialize mpz_ts for encrypting
    mpz_inits(m, print, NULL);
    while (feof(infile) == 0) {
        //read from infile, import, encrypt, then print to outfile
        j = fread(&bytes[1], sizeof(char), k - 1, infile);
        mpz_import(m, j + 1, 1, sizeof(uint8_t), 1, 0, bytes);
        ss_encrypt(print, m, n);
        gmp_fprintf(outfile, "%Zx\n", print);
    }
    //clear and free
    mpz_clears(m, print, NULL);
    free(bytes);
}

void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq) {
    pow_mod(m, c, d, pq);
}

void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t d, const mpz_t pq) {
    // calculate (log(pq) - 1) / 8
    size_t k = (mpz_sizeinbase(pq, 2) - 1) / 8;
    // allocate memory for an array of bytes
    uint8_t *bytes = (uint8_t *) calloc(k, sizeof(uint8_t));
    size_t j = 0;
    mpz_t c, m;
    mpz_inits(c, m, NULL);
    while (true) {
        // scan in file
        j = gmp_fscanf(infile, "%Zx", c);
        // if end of file, exit loop
        if (feof(infile)) {
            break;
        }
        //decrypt
        ss_decrypt(m, c, d, pq);
        // export to bytes and write it to outfile
        mpz_export(bytes, &j, 1, sizeof(uint8_t), 1, 0, m);
        fwrite(&bytes[1], sizeof(char), j - 1, outfile);
    }
    //free and clear
    mpz_clears(c, m, NULL);
    free(bytes);
}
