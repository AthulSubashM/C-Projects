# C Projects
## Number Generators
Random Dice Distribution - Generates 2 random die numbers for x trails which are added together and displays the resulting distribution on the terminal.
 
Normal Distribution - Generates X random numbers from a Normal distribution with a given mean and standard deviation using the Box-Muller transform method, and displays the resulting distribution on the terminal.

Poisson Distribution - Generates X random numbers from a Poisson distribution with a given  expected value (lamda) using the Knuth method, and displays the resulting distribution on the terminal.

Random Number Distribution - Generates X random numbers between 0 and 1, and displays the resulting distribution on the terminal.

## Scheduling Simulator
Job Generator - Generates three mixtures of job runtime for 5000 jobs with follows a Gaussian distribution. 
The three mixtures of jobs are written to three csv files along with a randomly generated job arrival time, in the format: job size, job arrival.

Scheduler - Reads three input files containing different mixtures of job sizes and arrival times and simulates five scheduling
algoritms, FIFO, SJF, STCF with pre-emption size of 30, RR with quantum size of 30 and RR with quantum size of 75.
The program then writes the job number, arrival time, status, job size, time left, start time, number of context
switches, end time, response time and turn around time for each job along with the the total number of context 
switches and average response and turnaround time for each algorithm to three files corresponding to the three
input files. 
