#include "mathlib.h"

static int terms;

/* calculates pi */
double pi_bbp(void) {
    /* variables to calculate pi */
    double k = 0.0;
    double approx_pi = 0.0;
    double pow_sixteen = 1.0;
    double prev_pi = 1.0;
    terms = 0;
    /* calculates pi using bbp and increases terms with each iteration */
    while (absolute(approx_pi - prev_pi) > EPSILON) {
        prev_pi = approx_pi;
        approx_pi
            += (1 / (pow_sixteen)
                * ((4 / (8 * k + 1)) - (2 / (8 * k + 4)) - (1 / (8 * k + 5)) - (1 / (8 * k + 6))));
        k++;
        pow_sixteen *= 16;
        terms++;
    }
    /* returns pi */
    return approx_pi;
}

/* resets terms to 0 and returns the value of terms */
int pi_bbp_terms(void) {
    int x = terms;
    terms = 0;
    return x;
}
