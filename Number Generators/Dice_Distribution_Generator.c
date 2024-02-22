/*******************************************************************************************************************************
 * Random Dice Distribution
 * 
 * Name: Dice_Distribution_Generator.c
 * 
 * Written by: Athul Subash Marottikkal - January 2023
 * 
 * Purpose: Generates 2 random die numbers for x trails which are added together and displays the resulting distribution on the terminal.
 * 
 * Usage: 
 * 	./Dice_Distribution_Generator
 * 	The program will prompt the user to enter the number of random numbers to be generated (x). The program will 
 * 	then generate and plot the distribution of the random die numbers.
 * 
 * Subroutines/libraries required: 
 * 	See include statements.
 * 	The program uses the rand() function from the stdlib.h library to generate random numbers, and 
 * 	also uses the time.h library to seed the random number generator with the current time.
 * 
*******************************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	int x = 0;				// Number of random numbers to be generated. 
	int index = 0;			// Corresponds to the index of the bin.
    int i = 0;				// Loop counter
	int bin[13] = {0};		// Initializes an array with the value of all elements set to 0. Used for binning.
	int bin_size = sizeof(bin) / sizeof(int);		// Stores the size of the array in integers.
    int d1 = 0, d2 = 0;		// Stores the randomly generated values for each of the two dice seperately

    srand(time(NULL));		// Seeds the rand() function with the current time.

	printf("Enter the number of random numbers to be generated: ");
	
	// Validates x.
	while(x <= 0) {
		scanf("%d", &x); 		// Reads the number of random numbers to be generated and stores it in x.
		if(x <= 0) {
			printf("\nError! Enter a positive number: ");
		}
	}

	/* Loop to generate x random die values for two dice, d1 and d2. The values are then added and stored at
	   the appropriate indices of the array, bin[].	*/
	for (i = 0; i < x; i++) {
		d1 = rand() % 6  + 1;		// Generates a random die value for the first die
		d2 = rand() % 6  + 1;		// Generates a random die value for the second die
		index = d1 + d2;			// Adds the values of the dice and stores it in the variable index
		bin[index]++;				// The value at this index of the array, bin[], is incremented by 1.
	}

	// Creates file for writing data from the array, bin[].
	FILE *fp;
	fp = fopen("p2_data.txt", "w");

	// Loop to write values from bin[] to the file.
	for (i = 0; i < bin_size; i++) {
		fprintf(fp, "%d\n", bin[i]);
	}
	fclose(fp);		// Closes file

	// Plots a random die number distribution graph on the terminal using gnuplot with the data from the generated file, p2_data.txt.
	FILE *gnuplot = popen("gnuplot", "w");
	fprintf(gnuplot, "set term dumb\n");
	fprintf(gnuplot, "set xlabel 'Bins' \n");
	fprintf(gnuplot, "set ylabel 'Frequency' \n");
	fprintf(gnuplot, "set xrange [0:14] \n");
	fprintf(gnuplot, "set yrange [0:*] \n");
	fprintf(gnuplot, "set style data histogram \n");
	fprintf(gnuplot, "set style histogram clustered \n");
	fprintf(gnuplot, "set xtics 0,1,12 \n");
	fprintf(gnuplot, "set key off \n");
	fprintf(gnuplot, "set title 'Die Number Distribution' \n");
	fprintf(gnuplot, "plot 'p2_data.txt' \n");
	pclose(gnuplot);

	return 0;
}	//eod

