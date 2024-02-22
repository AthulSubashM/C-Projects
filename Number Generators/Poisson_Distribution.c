/*******************************************************************************************************************************
 * Poisson Distribution
 * 
 * Name: Poisson_Distribution.c
 * 
 * Written by: Athul Subash Marottikkal - January 2023
 * 
 * Purpose: Generates X random numbers from a Poisson distribution with a given  expected value (lamda) using the Knuth method,
 * 			and displays the resulting distribution on the terminal.
 * 
 * Usage: 
 * 	./p4_pois_Poisson_Distributiondist
 * 	The program will prompt the user to enter the number of random numbers to be generated (x), expected value (lambda) of the
 *  Poisson distribution. The program will then generate and plot the Poisson distribution.
 * 
 * Subroutines/libraries required: 
 * 	See include statements.
 * 	The program uses the rand() function from the stdlib.h library to generate random numbers, and 
 * 	the roundl() function from the math.h library to round the numbers to the nearest bin index. 
 * 	The program also uses the time.h library to seed the random number generator with the current time.
 * 	Include the -lm option at compile time.
 * 	(gcc -o Poisson_Distribution Poisson_Distribution.c -lm)
 * 
*******************************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>

double scale_factor = 1.0 / (double)RAND_MAX; 	// Used to scale the values returned by the rand() function to [0,1]. Global variable.

int poisson_generator(double lamda);

int main() {
	int x = 0;				// Number of random numbers to be generated. 
	int index = 0;			// Corresponds to the index of the bin.
    int i = 0;				// Loop counter
	int bin[100] = {0};		// Initializes an array with the value of all elements set to 0. Used for binning.
	int bin_size = sizeof(bin) / sizeof(int);		// Stores the size of the array in integers.

	double lambda = 5;		// Expected value of the Poisson distribution. Default = 5.

	srand(time(NULL));		// Seeds the rand() function with the current time.

	printf("Enter the number of random numbers to be generated: ");

	// Validates x.
	do {
		scanf("%d", &x); 		// Reads the number of random numbers to be generated and stores it in x.
		if(x <= 0) {
			printf("\nError! Enter a positive number: ");
		}
	} while(x <= 0);

	printf("Enter the expected value: ");

	// Validates lambda.
	do {
		scanf("%lf", &lambda);	// Reads expected value and stores it in lambda.
		if(lambda <= 0) {
			printf("\nError! Enter a positive number: ");
		}
	} while(lambda <= 0);

	// Loop to generate x random numbers and store them, at the appropriate indices of the array, bin[].
	for(i = 0; i < x; i++) {
		index = poisson_generator(lambda) % bin_size;	/* Calls the poisson_generator() function with the
		parameter, lambda. A random value from a Poisson distribution with an expected value of lambda is returned and
		its remainder when divided by the bin_size is taken as the index */
		bin[index]++;	// The value at this index of the array, bin[], is incremented by 1.
	}

	// Creates file for writing data from the array, bin[].
	FILE *fp;
	fp = fopen("p4_data.txt", "w");

	// Loop to write values from bin[] to the file.
	for (i = 0; i < bin_size; i++) {
		if(bin[i] > 0) {
			fprintf(fp, "%d\n", bin[i]);
		}
	}
	fclose(fp);		// Closes file

	// Plots a random normal distribution graph on the terminal using gnuplot with the data from the generated file, p4_data.txt.
	FILE *gnuplot = popen("gnuplot", "w");
	fprintf(gnuplot, "set term dumb\n");
	fprintf(gnuplot, "set xlabel 'Bins' \n");
	fprintf(gnuplot, "set ylabel 'Frequency' \n");
	fprintf(gnuplot, "set xrange [0:*] \n");
	fprintf(gnuplot, "set yrange [0:*] \n");
	fprintf(gnuplot, "set style data histogram \n");
	fprintf(gnuplot, "set style histogram clustered \n");
	fprintf(gnuplot, "set key off \n");
	fprintf(gnuplot, "set title 'Poisson Distribution' \n");
	fprintf(gnuplot, "plot 'p4_data.txt'\n");
	pclose(gnuplot);
		
	return 0;
}	// eod


/**
 * This function generates random numbers from a Poisson distribution with an expected value of lambda using Knuth method. 
 * The function takes in one argument, lambda and outputs a random number from the Poisson distribution.
 * Source: https://en.wikipedia.org/wiki/Poisson_distribution#Computational_methods													*/ 
int poisson_generator(double lamda) {
	double L = exp(-lamda);
	int k = 0;
	double p = 1.0;

	do {
		k++;
		double u = (double)rand() * scale_factor ;
		p *= u;
	} while(p > L);

	return k-1;
}