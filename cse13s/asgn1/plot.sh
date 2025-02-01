#!/bin/bash

pi=3.141592643489793

make clean && make monte_carlo
./monte_carlo -n 5000 | tail -n 5000 | awk '{if ($5 == 0) print $3,$4,$5}' > /tmp/monte.dat
./monte_carlo -n 5000 | tail -n 5000 | awk '{if ($5 == 1) print $3,$4,$5}' > /tmp/monte1.dat 

gnuplot <<END
	set terminal pdf
	set output "dot.pdf" 
	set title ""
	set xlabel ""
	set ylabel ""
	set xrange [0:1]
	set yrange [0:1]
	set size square
	set pointsize 10
	plot sqrt(1 - x*x) lc "black" title "", "/tmp/monte.dat" using 1:2 with dots lc "red" title "", "/tmp/monte1.dat" using 1:2 with dots lc "blue" title ""
END
./monte_carlo -n 70000 | tail -n 70000 | awk '{print $1, $2 - '$pi'}' > /tmp/monte1.dat
sleep 1
./monte_carlo -n 70000 | tail -n 70000 | awk '{print $1, $2 - '$pi'}' > /tmp/monte2.dat
sleep 1
./monte_carlo -n 70000 | tail -n 70000 | awk '{print $1, $2 - '$pi'}' > /tmp/monte3.dat
sleep 1
./monte_carlo -n 70000 | tail -n 70000 | awk '{print $1, $2 - '$pi'}' > /tmp/monte4.dat
sleep 1
./monte_carlo -n 70000 | tail -n 70000 | awk '{print $1, $2 - '$pi'}' > /tmp/monte5.dat

gnuplot <<END 
	set terminal pdf
	set output "error.pdf"
	set title "Monte Carlo Error Estimation"
	set xlabel ""
	set ylabel "Error"
	set logscale x 4
	set xrange [1:70000]
	set yrange [-1:1]
	plot "/tmp/monte1.dat" using 1:2 with lines title "", "/tmp/monte2.dat" using 1:2 with lines title "", "/tmp/monte3.dat" using 1:2 with lines title "", "/tmp/monte3.dat" using 1:2 with lines title "", "/tmp/monte4.dat" using 1:2 with lines title "", "/tmp/monte5.dat" using 1:2 with lines title "", 0 with lines title ""
END
./monte_carlo -n 20 -r $RANDOM | tail -n 1 | awk '{print $2}' > /tmp/monte1.dat
./monte_carlo -n 20 -r $RANDOM | tail -n 1 | awk '{print $2}' > /tmp/monte2.dat
./monte_carlo -n 20 -r $RANDOM | tail -n 1 | awk '{print $2}' > /tmp/monte3.dat
./monte_carlo -n 20 -r $RANDOM | tail -n 1 | awk '{print $2}' > /tmp/monte4.dat
./monte_carlo -n 20 -r $RANDOM | tail -n 1 | awk '{print $2}' > /tmp/monte5.dat
./monte_carlo -n 20 -r $RANDOM | tail -n 1 | awk '{print $2}' > /tmp/monte6.dat
./monte_carlo -n 20 -r $RANDOM | tail -n 1 | awk '{print $2}' > /tmp/monte7.dat
./monte_carlo -n 20 -r $RANDOM | tail -n 1 | awk '{print $2}' > /tmp/monte8.dat
./monte_carlo -n 20 -r $RANDOM | tail -n 1 | awk '{print $2}' > /tmp/monte9.dat
./monte_carlo -n 20 -r $RANDOM | tail -n 1 | awk '{print $2}' > /tmp/monte10.dat
cat /tmp/monte1.dat /tmp/monte2.dat /tmp/monte3.dat /tmp/monte4.dat /tmp/monte5.dat /tmp/monte6.dat /tmp/monte7.dat /tmp/monte8.dat /tmp/monte9.dat /tmp/monte10.dat > /tmp/hist1.dat

./monte_carlo -n 20000 -r $RANDOM | tail -n 1 | awk '{print $2}' > /tmp/monte1.dat
./monte_carlo -n 20000 -r $RANDOM | tail -n 1 | awk '{print $2}' > /tmp/monte2.dat
./monte_carlo -n 20000 -r $RANDOM | tail -n 1 | awk '{print $2}' > /tmp/monte3.dat
./monte_carlo -n 20000 -r $RANDOM | tail -n 1 | awk '{print $2}' > /tmp/monte4.dat
./monte_carlo -n 20000 -r $RANDOM | tail -n 1 | awk '{print $2}' > /tmp/monte5.dat
./monte_carlo -n 20000 -r $RANDOM | tail -n 1 | awk '{print $2}' > /tmp/monte6.dat
./monte_carlo -n 20000 -r $RANDOM | tail -n 1 | awk '{print $2}' > /tmp/monte7.dat
./monte_carlo -n 20000 -r $RANDOM | tail -n 1 | awk '{print $2}' > /tmp/monte8.dat
./monte_carlo -n 20000 -r $RANDOM | tail -n 1 | awk '{print $2}' > /tmp/monte9.dat
./monte_carlo -n 20000 -r $RANDOM | tail -n 1 | awk '{print $2}' > /tmp/monte10.dat

cat /tmp/monte1.dat /tmp/monte2.dat /tmp/monte3.dat /tmp/monte4.dat /tmp/monte5.dat /tmp/monte6.dat /tmp/monte7.dat /tmp/monte8.dat /tmp/monte9.dat /tmp/monte10.dat > /tmp/hist2.dat

gnuplot <<END
	set terminal pdf
	set output "third.pdf"
	set xlabel ""
	set ylabel "Final estimate of pi"
	set xrange [-1:25]
	set yrange [2:4.2]
	set style histogram clustered
	set style fill solid 1 border -1
	set style data histograms
	plot newhistogram "20 Iterations" lt 3, "/tmp/hist1.dat" title "" , newhistogram "20000 Iterations" lt 2 at 15, "/tmp/hist2.dat" title "", 3.1415925 lc "black" title "PI"
END
