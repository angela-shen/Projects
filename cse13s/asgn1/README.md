Assignment 1 - Getting Acquainted with UNIX and C

Description: 

	monte_carlo.c is a program that generates randomized values of x and y between 0 and 1 to simulate an experiment that calculates the value of pi. plot.sh uses the output of monte_carlo.c to create graphs in gnuplot that visualize the data.
Build:

	To build monte_carlo.c, run $make or $make all on the command line. This will create the executable monte_carlo.
Cleaning: 

	To clean the directory, run $make clean to remove monte_carlo and monte_carlo.o.
Running:

	To run monte_carlo, type 
	./ monte_carlo
	Optionally, include the flags -n to specify the number of iterations or -r to specify the seed. 
	To run plot.sh type 
	chmod +x plot.sh 
	./plot.sh
	This will create three graphs as three .pdf files: dot.pdf, error.pdf, third.pdf.
Errors:

	No known errors. 
