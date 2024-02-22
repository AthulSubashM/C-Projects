/*******************************************************************************************************************************
 * Random Number Distribution
 * 
 * Name: Random_Distribution_Generator.c
 * 
 * Written by: Athul Subash Marottikkal - January 2023
 * 
 * Purpose: Generates X random numbers between 0 and 1, and displays the resulting distribution on the terminal.
 * 
 * Usage: 
 * 	./Random_Distribution_Generator
 * 	The program will prompt the user to enter the number of random numbers to be generated (x). The program will 
 * 	then generate and plot the distribution of the random numbers.
 * 
 * Subroutines/libraries required: 
 * 	See include statements.
 * 	The program uses the rand() function from the stdlib.h library to generate random numbers, and 
 * 	the roundl() function from the math.h library to round the numbers to the nearest bin index. 
 * 	The program also uses the time.h library to seed the random number generator with the current time.
 * 	Include the -lm option at compile time.
 * 	(gcc -o Random_Distribution_Generator Random_Distribution_Generator.c -lm)
 * 
*******************************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main() {
	int x = 0;				// Number of random numbers to be generated. 
	int index = 0;			// Corresponds to the index of the bin.
    int i = 0;				// Loop counter
	int bin[100] = {0};		// Initializes an array with the value of all elements set to 0. Used for binning.
	int bin_size = sizeof(bin) / sizeof(int);		// Stores the size of the array in integers.

    double scale_factor = 1.0 / (double) RAND_MAX;		// Used to scale the values returned by the rand() function to [0,1].

    srand(time(NULL));		// Seeds the rand() function with the current time.

	printf("Enter the number of random numbers to be generated: ");

	// Validates x.
	while(x <= 0) {
		scanf("%d", &x); 		// Reads the number of random numbers to be generated and stores it in x.
		if(x <= 0) {
			printf("\nError! Enter a positive number: ");
		}
	}

	// Loop to generate x random numbers and store them, at the appropriate indices of the array, bin[].
	for (i = 0; i < x; i++) {
		index = roundl((rand() * scale_factor) * bin_size); 	/* The value returned by the rand() function is multiplied by the
																   scale_factor and bin_size, and then rounded and stored in the index */ 
		bin[index]++;		// The value at this index of the array, bin[], is incremented by 1.
	}

	// Creates file for writing data from the array, bin[].
	FILE *fp;
	fp = fopen("p1_data.txt", "w");

	// Loop to write values from bin[] to the file.
	for (i = 0; i < bin_size; i++) {
		fprintf(fp, "%d\n", bin[i]);
	}
	fclose(fp);		// Closes file

	// Plots a random number distribution graph on the terminal using gnuplot with the data from the generated file, p1_data.txt.
	FILE *gnuplot = popen("gnuplot", "w");
	fprintf(gnuplot, "set term dumb\n");
	fprintf(gnuplot, "set xlabel 'Bins' \n");
	fprintf(gnuplot, "set ylabel 'Frequency' \n");
	fprintf(gnuplot, "set xrange [0:101] \n");
	fprintf(gnuplot, "set yrange [0:*] \n");
	fprintf(gnuplot, "set style data histogram \n");
	fprintf(gnuplot, "set style histogram clustered \n");
	fprintf(gnuplot, "set key off \n");
	fprintf(gnuplot, "set title 'Random Number Distribution' \n");
	fprintf(gnuplot, "plot 'p1_data.txt' \n");
	pclose(gnuplot);

	return 0;
} 	// eod

