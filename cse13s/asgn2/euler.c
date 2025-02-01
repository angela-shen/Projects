#include "mathlib.h"

static int terms;

/* calculates pi */
double pi_euler(void) {
    /* values to calculate pi */
    double k = 1.0;
    double prev_pi = 1.0;
    double approx_pi = 0.0;
    terms = 0;
    /* calculates pi using euler's formula */
    while (absolute(approx_pi - prev_pi) > EPSILON) {
        prev_pi = approx_pi;
        approx_pi += 1 / (k * k);
        k++;
        terms++;
    }
    /* returns pi rather than pi squared over 6 */
    return sqrt_newton(approx_pi * 6);
}

/* resets terms to 0 and returns the value of terms */
int pi_euler_terms(void) {
    int x = terms;
    terms = 0;
    return x;
}
