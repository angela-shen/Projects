#!/bin/bash

make clean && make all 

for i in $(seq 0 1 1000) ; do ./sorting -b -n $i ; done > batch.dat
for i in $(seq 0 1 1000) ; do ./sorting -h -n $i ; done > heap.dat
for i in $(seq 0 1 1000) ; do ./sorting -s -n $i ; done > shell.dat
for i in $(seq 0 1 1000) ; do ./sorting -q -n $i ; done > quick.dat

for i in $(seq 0 1 150) ; do ./sorting -b -n $i -r 1234 ; done > batch1.dat
for i in $(seq 0 1 150) ; do ./sorting -b -n $i -r 5678 ; done > batch2.dat
for i in $(seq 0 1 150) ; do ./sorting -b -n $i -r 9012 ; done > batch3.dat

for i in $(seq 0 1 150) ; do ./sorting -h -n $i -r 1234; done > heap1.dat
for i in $(seq 0 1 150) ; do ./sorting -h -n $i -r 5678; done > heap2.dat
for i in $(seq 0 1 150) ; do ./sorting -h -n $i -r 9012; done > heap3.dat

for i in $(seq 0 1 150) ; do ./sorting -s -n $i -r 1234; done > shell1.dat
for i in $(seq 0 1 150) ; do ./sorting -s -n $i -r 5678; done > shell2.dat
for i in $(seq 0 1 150) ; do ./sorting -s -n $i -r 9012; done > shell3.dat

for i in $(seq 0 1 150) ; do ./sorting -q -n $i -r 1234; done > quick1.dat
for i in $(seq 0 1 150) ; do ./sorting -q -n $i -r 5678; done > quick2.dat
for i in $(seq 0 1 150) ; do ./sorting -q -n $i -r 9012; done > quick3.dat

gnuplot <<END
	set terminal pdf
	set output "moves.pdf" 
	set title "Assorted Moves"
	set xlabel "number of elements"
	set ylabel "number of moves"
	set xrange [0:1000]
	set yrange [0:69000]
	plot 'heap.dat' using 1:2 with lines title "Heap Sort", 'batch.dat' using 1:2 with lines lt rgb "#39FF14" title "Batcher Sort" , 'shell.dat' using 1:2 with lines title "Shell Sort", 'quick.dat' using 1:2 with lines title "Quick Sort"
	
	set output "compare.pdf"
	set title "Assorted Compares"
	set ylabel "number of compares"
	set yrange [0:35000]
	plot 'heap.dat' using 1:3 with lines title "Heap Sort", 'batch.dat' using 1:3 with lines lt rgb "#39FF14" title "Batcher Sort" , 'shell.dat' using 1:3 with line title "Shell Sort", 'quick.dat' using 1:3 with line title "Quick Sort"
	
	set xrange [0:150]
	set output "heap.pdf"
	set title "Heap with Different Seeds"
	set ylabel "number of moves"
	set yrange [0:3500]
	plot 'heap1.dat' using 1:2 with lines title "", 'heap2.dat' using 1:2 with lines title "", 'heap3.dat' using 1:2 with lines title ""
	set ylabel "number of compares"
        set yrange [0:2000]
	plot 'heap1.dat' using 1:3 with lines title "", 'heap2.dat' using 1:3 with lines title "", 'heap3.dat' using 1:3 with lines title ""
	
	set output "batcher.pdf"
        set title "Batcher with Different Seeds"
        set ylabel "number of moves"
        set yrange [0:2000]
        plot 'batch1.dat' using 1:2 with lines title "", 'batch2.dat' using 1:2 with lines title "", 'batch3.dat' using 1:2 with lines title ""
        set ylabel "number of compares"
        set yrange [0:2500]
        plot 'batch1.dat' using 1:3 with lines title "", 'batch2.dat' using 1:3 with lines title "", 'batch3.dat' using 1:3 with lines title ""

	set output "shell.pdf"
        set title "Shell with Different Seeds"
        set ylabel "number of moves"
        set yrange [0:6000]
        plot 'shell1.dat' using 1:2 with lines title "", 'shell2.dat' using 1:2 with lines title "", 'shell3.dat' using 1:2 with lines title ""
        set ylabel "number of compares"
        set yrange [0:3000]
        plot 'shell1.dat' using 1:3 with lines title "", 'shell2.dat' using 1:3 with lines title "", 'shell3.dat' using 1:3 with lines title ""

	set output "quick.pdf"
        set title "Quick with Different Seeds"
        set ylabel "number of moves"
        set yrange [0:2500]
        plot 'quick1.dat' using 1:2 with lines title "", 'quick2.dat' using 1:2 with lines title "", 'quick3.dat' using 1:2 with lines title ""
        set ylabel "number of compares"
        set yrange [0:1500]
        plot 'quick1.dat' using 1:3 with lines title "", 'quick2.dat' using 1:3 with lines title "", 'quick3.dat' using 1:3 with lines title ""

END
