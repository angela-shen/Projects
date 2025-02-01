#include "numtheory.h"
#include "randstate.h"

#include <stdio.h>
#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>
#include <stdint.h>

void gcd(mpz_t g, const mpz_t a, const mpz_t b) {
    //based on assignment spec pseudocode
    /* finds mod of a and assigns it to b, then assigns a to b */
    /* mods by b to see if it divisible */
    /* continues until b is 0, meaning gcd has been found */
    mpz_t temp, a1, b1;
    mpz_inits(temp, a1, b1, NULL);
    mpz_set(a1, a);
    mpz_set(b1, b);
    while (mpz_cmp_ui(b1, 0) != 0 /*b != 0*/) {
        mpz_set(temp, b1); //uint64_t temp = b;
        mpz_mod(b1, a1, b1); //b = a % b;
        mpz_set(a1, temp); //a = temp;
    }
    mpz_set(g, a1);
    mpz_clears(temp, a1, b1, NULL);
}

void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n) {
    //based on assignment spec pseudocode
    mpz_t r, r_prime, t, t_prime, q, temp, extra;
    mpz_init_set(r, n); //int64_t r = n;
    mpz_init_set(r_prime, a); //int64_t r_prime = a;
    mpz_init_set_ui(t, 0); //int64_t t = 0;
    mpz_init_set_ui(t_prime, 1); //int64_t t_prime = 1;
    mpz_inits(q, temp, extra, NULL); //int64_t q;
    while (mpz_cmp_ui(r_prime, 0) != 0 /*r_prime != 0*/) {
        mpz_fdiv_q(q, r, r_prime); //q = r / r_prime;
        mpz_set(temp, r); //int64_t temp = r;
        mpz_set(r, r_prime); //r = r_prime;
        mpz_mul(extra, q, r_prime); //(q * r_prime);
        mpz_sub(r_prime, temp, extra); //r_prime = temp - (q * r_prime);
        mpz_set(temp, t); //temp = t;
        mpz_set(t, t_prime); //t = t_prime;
        mpz_mul(extra, q, t_prime);
        mpz_sub(t_prime, temp, extra); //t_prime = temp - (q * t_prime);
    }
    if (mpz_cmp_ui(r, 1) > 0 /*r > 1*/) {
        mpz_set_ui(o, 0); //return 0;
    }
    if (mpz_cmp_ui(t, 0) < 0 /*t < 0*/) {
        /* if negative inverse found, add n */
        /* because x % n == x + n % n */
        mpz_add(t, t, n); //t += n;
    }
    if (mpz_cmp_ui(r, 1) <= 0) {
        mpz_set(o, t); //return t;
    }
    mpz_clears(r, r_prime, t, t_prime, q, temp, extra, NULL);
}

void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n) {
    /* based on the pseudocode in the assignment 5 spec */
    mpz_t v, p, exp, extra;
    mpz_init_set_ui(v, 1); //int64_t v = 1;
    mpz_init_set(p, a); //int64_t p = a;
    mpz_init_set(exp, d); //exp = d
    mpz_init(extra);
    while (mpz_cmp_si(exp, 0) > 0 /*d > 0*/) {
        /* any positive integer can be represented by powers of 2 times a coefficient (either 1 or 0) */
        /* if d is odd, then coefficient of the power of 2 must be 1 */
        mpz_mod_ui(extra, exp, 2);
        if (mpz_cmp_ui(extra, 1) == 0 /*d % 2 == 1*/) {
            /* v will equal the base raised to the powers of 2 with coefficients of 1 */
            /* mod by n because this may produce a large number */
            mpz_mul(extra, v, p);
            mpz_mod(v, extra, n); //v = (v * p) % n;
        }
        /* p * p represents p^2 (powers of 2 representing exponent d), so this increases the power of 2 */
        /* mod by n because this may produce a large number */
        mpz_mul(extra, p, p);
        mpz_mod(p, extra, n); //p = (p * p) % n;
        mpz_fdiv_q_ui(exp, exp, 2); //d /= 2;
    }
    mpz_set(o, v); //return v;
    mpz_clears(v, p, exp, extra, NULL);
}

bool is_prime(const mpz_t n, uint64_t iters) {
    //based on the pseudocode in the assignment 5 spec
    /* prevent cases smaller than 5 */
    if (mpz_cmp_ui(n, 1) == 0 || mpz_cmp_ui(n, 0) == 0 || mpz_cmp_ui(n, 4) == 0) {
        return false;
    }
    if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0) {
        return true;
    }
    /* Miller-Rabin: */
    /* if a^r % n and a^2s(r) % n are not equal to 1 or n - 1 */
    /* n is composite, otherwise might be prime */
    mpz_t r, s, a, pow_two, extra, y, s_minus;
    //uint64_t r = 0;
    //uint64_t s = 0;
    //uint64_t a = 0;
    //also s - 1
    mpz_inits(r, s, a, s_minus, NULL);

    /* find s such that n - 1 = 2^s and r is odd */
    mpz_init_set_ui(pow_two, 2); //uint64_t pow_two = 2;
    mpz_inits(extra, y, NULL);
    mpz_mod_ui(extra, r, 2);
    while (mpz_cmp_ui(extra, 1) != 0 /*r % 2 != 1*/) {
        mpz_sub_ui(extra, n, 1); // set extra to n - 1
        mpz_fdiv_q(r, extra, pow_two); // r = (n - 1 / pow_two);
        mpz_add_ui(s, s, 1); //s++;
        mpz_mul_ui(pow_two, pow_two, 2); //pow_two *= 2;
        mpz_mod_ui(extra, r, 2); //set extra to r % 2
    }

    /* performs miller rabin test iters amount of times */
    for (uint64_t i = 0; i < iters; i++) {
        /* generate a "witness" that is between 2 and n - 2 */
        mpz_sub_ui(extra, n, 3);
        mpz_urandomm(a, state, extra); //a = random();
        mpz_add_ui(a, a, 2); //randomm is between 0 and n - 4, add 2

        pow_mod(y, a, r, n); //uint64_t y = pow_mod(a, r, n);
        mpz_sub_ui(extra, n, 1); // extra = n - 1

        if (mpz_cmp_ui(y, 1) != 0 /*y != 1*/ && mpz_cmp(y, extra) != 0 /*y != (n - 1)*/) {
            /* check if a^2s(r) is 1 or n - 1 for all values up to s - 1 */
            mpz_sub_ui(s_minus, s, 1);
            for (uint64_t j = 1; mpz_cmp_ui(s_minus, j) >= 0
                                 && mpz_cmp(extra, y) != 0 /* j <= (s - 1) && y != (n - 1) */;
                 j++) {
                mpz_set_ui(extra, 2);
                pow_mod(y, y, extra, n); //y = pow_mod(y, 2, n);
                if (mpz_cmp_ui(y, 1) == 0 /*y == 1*/) {
                    mpz_clears(r, s, a, pow_two, extra, s_minus, y, NULL);
                    return false;
                }
                mpz_sub_ui(extra, n, 1);
            }
            mpz_sub_ui(extra, n, 1);
            if (mpz_cmp(y, extra) != 0 /*y != n - 1*/) {
                mpz_clears(r, s, a, pow_two, extra, s_minus, y, NULL);
                return false;
            }
        }
    }
    mpz_clears(r, s, a, pow_two, extra, s_minus, y, NULL);
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    //based pseudocode from the assignment spec
    //uint64_t p = 0;
    bool prime = false;
    uint64_t min = 1;
    min <<= (bits - 1); // calculate 2 ^ (bits - 1)
    /* end loop when prime is found */
    while (!(prime)) {
        //p = random();
        mpz_urandomb(p, state, bits); // generates number between 0 and 2^(bits - 1)
        mpz_add_ui(p, p, min); //add 2 ^ bits - 1 to guarantee at least bits amount of bits
        /* check if it is prime and greater than 4 (so p cannot be 2 or 3) */
        prime = is_prime(p, iters);
    }
    //return p;
}
