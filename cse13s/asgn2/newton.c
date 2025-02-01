#include "mathlib.h"

static int terms;

/* calculates sqrt */
double sqrt_newton(double x) {
    double f = 1.0;
    terms = 1;
    /* scales down the value of x if it is greater than 4 */
    /* sqrt (x / 4) is equal to sqrt(x) / 2*/
    /* so dividing by 4 and multiplying by 2 keeps the values proportional */
    while (x > 4) {
        x /= 4.0;
        f *= 2.0;
    }
    /* some initial guesses, 1 and 0*/
    double y = 1.0;
    double z = 0.0;
    /* implements newton-raphson formula to estimate the sqrt */
    while (absolute(y - z) > EPSILON) {
        z = y;
        y = (y + x / y) / 2.0;
        terms++;
    }
    /* multiplies the estimate by previously calculated scale factor */
    return f * y;
}

/* resets terms to 0 and returns the value of terms */
int sqrt_newton_iters(void) {
    int x = terms;
    terms = 0;
    return x;
}
