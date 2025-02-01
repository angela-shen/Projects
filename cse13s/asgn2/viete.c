#include "mathlib.h"

static int terms;

/* calculates pi */
double pi_viete(void) {
    /* variables to calculate pi */
    double num = sqrt_newton(2.0);
    double approx_pi = 1.0;
    double prev_pi = 0.0;
    terms = 0;
    /* calculates pi using viete's formula, increases terms by 1 each iteration*/
    while (absolute(approx_pi - prev_pi) > EPSILON) {
        prev_pi = approx_pi;
        approx_pi *= (num / 2);
        num = sqrt_newton(2 + num);
        terms++;
    }
    /* returns pi instead of 2/pi */
    return 2 * (1 / approx_pi);
}

/* resets terms to 0 and returns the value of terms */
int pi_viete_factors(void) {
    int x = terms;
    terms = 0;
    return x;
}
