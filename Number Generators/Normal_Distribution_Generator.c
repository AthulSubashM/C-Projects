/*******************************************************************************************************************************
 * Normal Distribution
 * 
 * Name: Normal_Distribution_Generator.c
 * 
 * Written by: Athul Subash Marottikkal - January 2023
 * 
 * Purpose: Generates X random numbers from a Normal distribution with a given mean and standard deviation using the Box-Muller
 * 			transform method, and displays the resulting distribution on the terminal.
 * 
 * Usage: 
 * 	./Normal_Distribution_Generator
 * 	The program will prompt the user to enter the number of random numbers to be generated (x), the mean and the standard deviation
 *  of the Normal distribution. The program will then generate and plot the Normal distribution.
 * 
 * Subroutines/libraries required: 
 * 	See include statements.
 * 	The program uses the rand() function from the stdlib.h library to generate random numbers, and 
 * 	the roundl() function from the math.h library to round the numbers to the nearest bin index. 
 * 	The program also uses the time.h library to seed the random number generator with the current time.
 * 	The _USE_MATH_DEFINES macro is used to make M_PI available for use in the program and the float.h library
 *  is used to make DBL_EPSILON available.
 * 	Include the -lm option at compile time.
 * 	(gcc -o Normal_Distribution_Generator Normal_Distribution_Generator.c -lm)
 * 
*******************************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<float.h>
#define _USE_MATH_DEFINES
#include <math.h>

double scale_factor = 1.0 / (double)RAND_MAX; 	// Used to scale the values returned by the rand() function to [0,1]. Global variable.

double rand_generator(double mean, double sd);

int main() {
	int x = 0;				// Number of random numbers to be generated. 
	int index = 0;			// Corresponds to the index of the bin.
    int i = 0;				// Loop counter
	int bin[100] = {0};		// Initializes an array with the value of all elements set to 0. Used for binning.
	int bin_size = sizeof(bin) / sizeof(int);		// Stores the size of the array in integers.

	double mean = 12;	// Mean of the normal distribution. Default = 12.
	double sd = 2;		// Standard deviation of the normal distribution. Default = 2.

	srand(time(NULL));		// Seeds the rand() function with the current time.

	printf("Enter the number of random numbers to be generated: ");

	// Validates x.
	while(x <= 0) {
		scanf("%d", &x); 		// Reads the number of random numbers to be generated and stores it in x.
		if(x <= 0) {
			printf("\nError! Enter a positive number: ");
		}
	}

	printf("Enter the mean: ");
	scanf("%lf", &mean);	// Reads the mean and stores it in the variable mean.
	printf("Enter the standard deviation: ");
	scanf("%lf", &sd);	// Reads the standard deviation and stores it in the variable sd.

	// Empirical rule. Source: https://en.wikipedia.org/wiki/68%E2%80%9395%E2%80%9399.7_rule
	double min = mean - 3 * sd;		// Generates an estimated minimum value for the distribution.
	double max = mean + 3 * sd;		// Generates an estimated maximum value for the distribution.

	double bin_width = 100 / (max - min);	// Calculates the width of each bin

	// Loop to generate x random numbers and store them, at the appropriate indices of the array, bin[].
	for(i = 0; i < x; i++)
	{
		index = roundl(bin_width * (rand_generator(mean,sd) - min));	/* Calls the rand_generator() function with the
		parameters, mean and sd. The returned normally random value is subtracted by the min value and multiplied by bin_width
		 to get the appropriate index */

		/* The if and else if statements stops the index from being out of the array's bounds. Accounts for around 0.3% of 
		   the values that are outside the Emirical rule */  
		if(index > 99)
			index = 99;
		else if(index < 0)
			index = 0;

		bin[index]++;	// The value at this index of the array, bin[], is incremented by 1.
	}

	// Creates file for writing data from the array, bin[].
	FILE *fp;
	fp = fopen("p3_data.txt", "w");

	// Loop to write values from bin[] to the file.
	for (i = 0; i < bin_size; i++) {
		fprintf(fp, "%d\n", bin[i]);
	}
	fclose(fp);		// Closes file

	// Plots a random normal distribution graph on the terminal using gnuplot with the data from the generated file, p3_data.txt.
	FILE *gnuplot = popen("gnuplot", "w");
	fprintf(gnuplot, "set term dumb\n");
	fprintf(gnuplot, "set xlabel 'Bins' \n");
	fprintf(gnuplot, "set ylabel 'Frequency' \n");
	fprintf(gnuplot, "set xrange [0:101] \n");
	fprintf(gnuplot, "set yrange [0:*] \n");
	fprintf(gnuplot, "set style data histogram \n");
	fprintf(gnuplot, "set style histogram clustered \n");
	fprintf(gnuplot, "set key off \n");
	fprintf(gnuplot, "set title 'Normal Distribution' \n");
	fprintf(gnuplot, "plot 'p3_data.txt'\n");
	pclose(gnuplot);
		
	return 0;
}	// eod


/**
 * This function generates normally distributed random numbers using the Box-Muller transform method. The function takes in 
 * two arguments, mean and standard deviation (sd) and outputs a random number from the normal distribution.
 * Source: https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform 														*/ 
double rand_generator(double mean, double sd) {
	double u1 = 0, u2 = 0;		// Stores the uniformly distributed random numbers
	double z = 0;				// Stores the randomly generated number after transformation
	
	// The value returned by the rand() function is multiplied by the scale_factor and stored in u1 and u2.
	u1 = rand() * scale_factor;
	u2 = rand() * scale_factor;

	// This if statement gets rid of infinities caused by the log function when u1 is less than zero.
	if (u1 < DBL_EPSILON) {
        u1 = DBL_EPSILON;
    }

	z = sqrt(-2 * log(u1)) * cos(2 * M_PI * u2);	// Applies the Box-Muller transform formula and stores the value in z

	return mean + sd * z;		// returns the normally distributed random number.
}
