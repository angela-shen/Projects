#include "mathlib.h"

static int terms;

/* calculates pi */
double pi_madhava(void) {
    /* values used to calculate pi*/
    double k = 0.0;
    double num = 1.0;
    double approx_pi = 0.0;
    double prev_pi = 1.0;
    /* no terms calculated yet*/
    terms = 0;
    /* calculates pi using madhava formula, increases terms by 1 */
    while (absolute(approx_pi - prev_pi) > EPSILON) {
        prev_pi = approx_pi;
        approx_pi += (1 / (num * (2 * k + 1)));
        num *= -3;
        k++;
        terms++;
    }
    /* returns pi instead of pi / (sqrt(12)) */
    return approx_pi * sqrt_newton(12.0);
}

/* resets terms to 0 and returns the value of terms */
int pi_madhava_terms(void) {
    int x = terms;
    terms = 0;
    return x;
}
