#include "mathlib.h"

#include <math.h>
#include <stdio.h>
#include <unistd.h>
/* implements options */
#define OPTIONS "aebmrvnsh"

/* prints the help menu for mathlib-test */
void help(char *exec) {
    /*implement help menu based on resources binary and monte_carlo*/
    printf("SYNOPSIS\n"
           "       A test harness for the small numerical library.\n"
           "\n"
           "USAGE\n"
           "       %s [-aebmrvnsh]\n"
           "\n"
           "OPTIONS\n"
           "       -a      Runs all tests.\n"
           "       -e      Runs e test.\n"
           "       -b      Runs BBP pi test.\n"
           "       -m      Runs Madhava pi test.\n"
           "       -r      Runs Euler pi test.\n"
           "       -v      Runs Viete test.\n"
           "       -n      Runs Newton square root tests.\n"
           "       -s      Print verbose statistics.\n"
           "       -h      Display program synopsis and usage.\n",
        exec);
}

/* main function */
int main(int argc, char **argv) {
    /* bools for the switch */
    int e1, b, m, r, v, n, s, h, d = 0;
    /* initializes opt */
    int opt = 0;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        /* a sets all bools to 1 to perform all tests */
        case 'a':
            e1 = 1;
            b = 1;
            m = 1;
            r = 1;
            v = 1;
            n = 1;
            break;
        /* does individual tests*/
        case 'e': e1 = 1; break;
        case 'b': b = 1; break;
        case 'm': m = 1; break;
        case 'r': r = 1; break;
        case 'v': v = 1; break;
        case 'n': n = 1; break;
        /* statistics */
        case 's': s = 1; break;
        /* if h, nothing, or invalid no more tests, just print help*/
        case 'h': h = 1; break;
        default:
            d = 1;
            help(argv[0]);
            break;
        }
    }
    /* if no test args were given OR help was called AND default was not called, then call help */
    if (((e1 == 0 && b == 0 && m == 0 && r == 0 && v == 0 && n == 0) || h == 1) && d != 1) {
        e1 = 0;
        b = 0;
        m = 0;
        r = 0;
        v = 0;
        n = 0;
        help(argv[0]);
    }

    /* uses booleans and does stuff*/
    double pi = M_PI;
    double math_e = M_E;
    /* if s, all tests must have terms */
    if (s != 0) {
        if (e1 != 0) {
            double e_approx = e();
            printf("e() = %16.15lf , M_E = %16.15lf , diff = %16.15lf \n"
                   "e() terms = %d\n",
                e_approx, math_e, absolute(e_approx - math_e), e_terms());
        }
        if (b != 0) {
            double b = pi_bbp();
            printf("pi_bbp() = %16.15lf , M_PI = %16.15lf , diff = %16.15lf \n"
                   "pi_bbp() terms = %d\n",
                b, pi, absolute(b - pi), pi_bbp_terms());
        }
        if (m != 0) {
            double m = pi_madhava();
            printf("pi_madhava() = %16.15lf , M_PI = %16.15lf , diff = %16.15lf \n"
                   "pi_madhava() terms = %d\n",
                m, pi, absolute(m - pi), pi_madhava_terms());
        }
        if (r != 0) {
            double r = pi_euler();
            printf("pi_euler() = %16.15lf , M_PI = %16.15lf , diff = %16.15lf \n"
                   "pi_euler() terms = %d\n",
                r, pi, absolute(r - pi), pi_euler_terms());
        }
        if (v != 0) {
            double v = pi_viete();
            printf("pi_viete() = %16.15lf , M_PI = %16.15lf , diff = %16.15lf \n"
                   "pi_viete() terms = %d\n",
                v, pi, absolute(v - pi), pi_viete_factors());
        }
        if (n != 0) {
            for (double i = 0.0; i < 10.0; i += 0.2) {
                double n = sqrt_newton(i);
                printf("sqrt_newton(%16.15lf) = %16.15lf , sqrt(%16.15lf) = %16.15lf , diff = "
                       "%16.15lf \n"
                       "sqrt_newton() terms = %d\n",
                    i, n, i, sqrt(i), absolute(n - sqrt(i)), sqrt_newton_iters());
            }
        }
    } else {
        if (e1 != 0) {
            double e_approx = e();
            printf("e() = %16.15lf , M_E = %16.15lf , diff = %16.15lf \n", e_approx, math_e,
                absolute(e_approx - math_e));
        }
        if (b != 0) {
            double b = pi_bbp();
            printf("pi_bbp() = %16.15lf , M_PI = %16.15lf , diff = %16.15lf \n", b, pi,
                absolute(b - pi));
        }
        if (m != 0) {
            double m = pi_madhava();
            printf("pi_madhava() = %16.15lf , M_PI = %16.15lf , diff = %16.15lf \n", m, pi,
                absolute(m - pi));
        }
        if (r != 0) {
            double r = pi_euler();
            printf("pi_euler() = %16.15lf , M_PI = %16.15lf , diff = %16.15lf \n", r, pi,
                absolute(r - pi));
        }
        if (v != 0) {
            double v = pi_viete();
            printf("pi_viete() = %16.15lf , M_PI = %16.15lf , diff = %16.15lf \n", v, pi,
                absolute(v - pi));
        }
        if (n != 0) {
            for (double i = 0.0; i < 10.0; i += 0.1) {
                double n = sqrt_newton(i);
                printf("sqrt_newton(%16.15lf) = %16.15lf , sqrt(%16.15lf) = %16.15lf , diff = "
                       "%16.15lf \n",
                    i, n, i, sqrt(i), absolute(n - sqrt(i)));
            }
        }
    }
    return 0;
}
