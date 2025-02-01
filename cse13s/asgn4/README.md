### Assignment 3 - A little slice of pi

## Description

Implements Conway's Game of Life. The function main() in life.c implements the game and parses the command line arguments, universe.c provides the structure Universe and the associated commands needed for life.c

## Building

Run $make, $make life, or $make all on the command line. This will create the executable "life" and the necessary object files (life.o and universe.o) for it to run.

## Running

To run sorting, type ./life. Optionally, include the following flags: -h prints the help message, -t sets the universe as a toroidal, -s silences ncurses so that nothing will be displayed by ncurses, -n allows the user to specify how many generations the program should run, -i allows the user to specify the input file, and -o allows the user to specify the output file.

## Known errors

Running make scanbuild generates a few warnings, but does not seem to generate any errors.
Valgrind generates errors when ncurses is not silenced, but running ./life -s has no memory leaks or errors.

## DESIGN.pdf

Describes the pseudocode for life.c and universe.c

## WRITEUP.pdf

An analysis of what I learned from this assignment.
