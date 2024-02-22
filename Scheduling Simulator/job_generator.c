/*******************************************************************************************************************************
 * Job Generator
 * 
 * Name: job_generator.c
 * 
 * Written by: Athul Subash Marottikkal - February 2023
 * 
 * Purpose: Generates three mixtures of job runtime for 5000 jobs with follows a Gaussian distribution:
 *              1) mean of 150 units and standard deviation of 20 units.
 *              2) Two classes of jobs with 20% of the jobs with mean of 250 units and standard deviation of 15 units and the 
 *                 other 80% with mean of 50 units and standard deviation of 10 units.
 *              3) Two classes of jobs with 80% of the jobs with mean of 250 units and standard deviation of 15 units and the 
 *                 other 20% with mean of 50 units and standard deviation of 10 units.
 *          The three mixtures of jobs are written to three csv files along with a randomly generated job arrival time from a
 *          Guassian distribution with a mean of 75 units and standard deviation of 20 units. The jobs are written in the 
 *          format: job size, job arrival.
 * 
 * 
 * Usage: ./job_generator
 * 
 * Subroutines/libraries required: 
 * 	See include statements.
 * 	The program uses the rand() function from the stdlib.h library to generate random numbers. 
 * 	The program uses the time.h library to seed the random number generator with the current time.
 * 	The _USE_MATH_DEFINES macro is used to make M_PI available for use in the program and the float.h library
 *  is used to make DBL_EPSILON available.
 * 	Include the -lm option at compile time.
 * 	(gcc job_generator.c -o job_generator -lm)
 * 
*******************************************************************************************************************************/
#include<float.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define NO_OF_JOBS 5000                         // Sets value for the number of jobs to be generated.

float scale_factor = 1.0 / (float)RAND_MAX; 	// Used to scale the values returned by the rand() function to [0,1]. Global variable.

float rand_generator(float mean, float sd);

int main() 
{
    FILE *f1, *f2, *f3;

    int i = 0;              // Loop counter.
	float arrival_time;     // Job arrival time.
    float job_size;         // Job runtime.
    float split;            // Used to create the split ratio.

    // Opens files for writting.
    f1 = fopen("job_runtime_1.csv", "w");
    f2 = fopen("job_runtime_2.csv", "w");
    f3 = fopen("job_runtime_3.csv", "w");

    // Prints and error message and exits the program if the output file(s) cannot be openned.
    if (!(f1 && f2 && f3)) 
    {           
        perror("Error! Cannot open output file(s)!");
        exit(-1);
    }
    

    srand(time(NULL));		// Seeds the rand() function with the current time.

	// Loop to generate and write to file the arrival time and job runtime for the first mixture of jobs.
	for(arrival_time = 0.0, i = 0; i < NO_OF_JOBS; i++) 
    {		
        job_size = rand_generator(150, 20);
        arrival_time = arrival_time + rand_generator(75, 20);
        fprintf(f1, "%.2f,%.2f\n", job_size, arrival_time);
	}

    // Loop to generate and write to file the arrival time and job runtime for the second mixture of jobs.
    for(arrival_time = 0.0, i = 0; i < NO_OF_JOBS; i++) 
    {
        split = rand() * scale_factor;

        // Splits jobs into two classes.
        if(split > 0.8) {
            job_size = rand_generator(250, 15);
        }       
        else {
            job_size = rand_generator(50, 10);
        }

		arrival_time = arrival_time + rand_generator(75, 20);
        fprintf(f2, "%.2f,%.2f\n", job_size, arrival_time);
	}

    // Loop to generate and write to file the arrival time and job runtime for the third mixture of jobs.
    for(arrival_time = 0.0, i = 0; i < NO_OF_JOBS; i++) 
    {
        split = rand() * scale_factor;

        // Splits jobs into two classes.
        if(split < 0.8) {
            job_size = rand_generator(250, 15);
        }       
        else {
            job_size = rand_generator(50, 10);
        }

		arrival_time = arrival_time + rand_generator(75, 20);
        fprintf(f3, "%.2f,%.2f\n", job_size, arrival_time);
	}


    // Closes the files.
    fclose(f1);
    fclose(f2);
    fclose(f3);

    exit(0);
}



/***************************************************************************************************************************
 * Function Name: rand_generator()
 * Arguments: Two float variables - mean and sd.
 * Description: This function generates normally distributed random numbers using the Box-Muller transform method. Outputs 
 *              a random number from the normal distribution.
 * Source: https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform 											
 * *************************************************************************************************************************/ 

float rand_generator(float mean, float sd) 
{
	float u1 = 0, u2 = 0;		// Stores the uniformly distributed random numbers
	float z = 0;				// Stores the randomly generated number after transformation
	
	// The value returned by the rand() function is multiplied by the scale_factor and stored in u1 and u2.
	u1 = rand() * scale_factor;
	u2 = rand() * scale_factor;

	// This if statement gets rid of infinities caused by the log function when u1 is less than zero.
	if (u1 < DBL_EPSILON) {
        u1 = DBL_EPSILON;
    }

	z = sqrt(-2 * log(u1)) * cos(2 * M_PI * u2);	// Applies the Box-Muller transform formula and stores the value in z.
	return mean + sd * z;		                    // returns the normally distributed random number.
}
	
