#include "mathlib.h"

static int terms;

/* calculate e */
double e(void) {
    /* values used to calculate e */
    double k_fact = 1.0;
    double k = 1.0;
    double approx_e = 1.0;
    double prev_e = 0.0;
    /* because approx_e = 1.0 is one term, terms starts at 1 */
    terms = 1;
    /* calculates e using summation of 1/k!, increases terms by 1 each iteration */
    while (absolute(approx_e - prev_e) > EPSILON) {
        prev_e = approx_e;
        approx_e += 1 / k_fact;
        k++;
        k_fact *= k;
        terms++;
    }
    return approx_e;
}
/* resets terms to 0 just in case and returns the value of terms */
int e_terms(void) {
    int x = terms;
    terms = 0;
    return x;
}
