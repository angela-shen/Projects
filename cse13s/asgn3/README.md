### Assignment 3 - A little slice of pi

## Description

sorting.c is a program that tests various sorting algorithms. It uses functions from shell.c, batcher.c, heap.c, quick.c, stats.c and set.c.

## Building

Run $make or $make all on the command line. This will create the executable "sorting" and the necessary object files for it to run.

## Running

To run sorting, type ./sorting. Optionally, include the following flags: -H prints the help message, -a runs all tests, -s runs shells sort, -b runs batcher sort, -q runs quick sort, -h runs heap sort, -n lets the user change how many items are in the array, -p lets the user change how many items are printed, and -r lets the user change the seed.

## Known errors

Running make scanbuild generates a few warnings about values not being read from a variable in a function, but this does not seem to generate any errors. 

## DESIGN.pdf

Describes the pseudocode for all c files.

## WRITEUP.pdf

An analysis of the results of the assignment.
Contains graphs written in gnuplot.

## plot.sh

Script for the graphs in WRITEUP.pdf
