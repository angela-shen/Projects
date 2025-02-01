### Assignment 2 - A little slice of pi

## Description

mathlib-test.c is a program that uses various functions from a library, mathlib.h, and compares them to constants from math.h.

## Building

Run $make or $make all on the command line. This will create the executable "mathlib-test" and the necessary object files for it to run

## Running

To run mathlib-test, type ./mathlib-test. 
Optionally, include the following flags: -a runs all tests, -e runs tests for calculating euler's number, -b runs the BBP pi test, -m runs the Madhava pi test, -r runs the Euler pi test, -v runs the Viete pi test, -n runs tests for Newton's square root method, -s prints statistics for all of the above tests, and -h prints the program's synopsis and help menu.

## Known errors

Running make scanbuild generates 3 errors, which do not come up during compilation but refers to a handful of variables as "garbage variables".


