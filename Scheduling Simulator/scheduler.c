/*******************************************************************************************************************************
 * Job Generator
 * 
 * Name: scheduler.c
 * 
 * Written by: Athul Subash Marottikkal - February 2023
 * 
 * Purpose: Reads three input files containing different mixtures of job sizes and arrival times and simulates five scheduling
 *          algoritms, FIFO, SJF, STCF with pre-emption size of 30, RR with quantum size of 30 and RR with quantum size of 75.
 *          The program then writes the job number, arrival time, status, job size, time left, start time, number of context
 *          switches, end time, response time and turn around time for each job along with the the total number of context 
 *          switches and average response and turnaround time for each algorithm to three files corresponding to the three
 *          input files. 
 * 
 * Usage: ./scheduler
 * 
 * Subroutines/libraries required: 
 * 	See include statements.
 * 
*******************************************************************************************************************************/
#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include <sys/stat.h>

#define NO_OF_JOBS 5000     // Sets value for the number of jobs to be generated.

// Structure contains each job's details.
struct Job
{
    int job_no;
    float arrival_time;
    int status;     // 0: Not arrived, 1: Arrived & 2: Completed. 
    float size;
    float time_left;
    float start_time;
    int context_switch;
    float end_time;
    float response_time;
    float turnaround;
};

void Scheduler(FILE *fin, FILE *fout);
void FIFO(struct Job *queue, FILE *fin);                                                                
void SJF(struct Job *queue, FILE *fin);
void STCF(struct Job *queue, FILE *fin, int pre_emption_size);
void RR(struct Job *queue, FILE *fin, int quantum_size);


int main()
{
    FILE *fi1, *fi2, *fi3, *fo1, *fo2, *fo3;

    // Opens the input files containing the job arrival time and job size for reading.
    fi1 = fopen("job_runtime_1.csv", "r");
    fi2 = fopen("job_runtime_2.csv", "r");
    fi3 = fopen("job_runtime_3.csv", "r");

    // Prints and error message and exits the program if the input file(s) cannot be openned.
    if(!(fi1 && fi2 && fi3))
    {
        perror("Error! Cannot open input file(s)!");
        exit(-1);
    }

    // Opens output files for writing.
    fo1 = fopen("scheduling_1.csv", "w");
    fo2 = fopen("scheduling_2.csv", "w");
    fo3 = fopen("scheduling_3.csv", "w");

    // Writes the job mixture to the output files.
    fprintf(fo1, "Guassian,Mean: 150, SD: 20\n");
    fprintf(fo2, "20%% larger,Mean: 250, SD: 15\n");
    fprintf(fo2, "80%% smaller,Mean: 50, SD: 10\n");
    fprintf(fo3, "80%% larger,Mean: 50, SD: 10\n");
    fprintf(fo3, "20%% smaller,Mean: 250, SD: 15\n");

    // Prints and error message and exits the program if the output file(s) cannot be openned.
    if(!(fo1 && fo2 && fo3))
    {
        perror("Error! Cannot open output file(s)!");
        exit(-1);
    }

    Scheduler(fi1, fo1);
    Scheduler(fi2, fo2);
    Scheduler(fi3, fo3);

    // Closes all the files.
    fclose(fi1);
    fclose(fi2);
    fclose(fi3);  
    fclose(fo1);
    fclose(fo2);
    fclose(fo3);

    exit(0);
}



/***************************************************************************************************
 * Function Name: Scheduler()
 * Arguments: fin - File containing job arrival time and job size.
 *            fout - File used to write job details.
 * Description: Function to call the scheduling alogrithms, compute the average response and turnaround
 *              time and the total number of context switches, and write the the job details to a csv file.
 * *************************************************************************************************/
void Scheduler(FILE *fin, FILE *fout)
{   
    // Creates array of type structure Job to contains the job details for each scheduling algorithm.
    struct Job FIFO_queue[NO_OF_JOBS];
    struct Job SJF_queue[NO_OF_JOBS];
    struct Job STCF_queue[NO_OF_JOBS];
    struct Job RR1_queue[NO_OF_JOBS];
    struct Job RR2_queue[NO_OF_JOBS];

    // Calls the scheduling algorithms.
    FIFO(FIFO_queue, fin);
    SJF(SJF_queue, fin);
    STCF(STCF_queue, fin, 30);
    RR(RR1_queue, fin, 30);
    RR(RR2_queue, fin, 75);

    int i;

    // Variables to store the the average response and turnaround time ,and the total number of context switches
    // for each scheduling algorithm.
    float avg_response_FIFO = 0;
    float avg_turnaround_FIFO = 0;
    int total_context_switches_FIFO = 0;

    float avg_response_SJF = 0;
    float avg_turnaround_SJF = 0;
    int total_context_switches_SJF = 0;

    float avg_response_STCF = 0;
    float avg_turnaround_STCF = 0;
    int total_context_switches_STCF = 0;

    float avg_response_RR1 = 0;
    float avg_turnaround_RR1 = 0;
    int total_context_switches_RR1 = 0;

    float avg_response_RR2 = 0;
    float avg_turnaround_RR2 = 0;
    int total_context_switches_RR2 = 0;

    // Computes the average response and turnaround time ,and the total number of context switches for each scheduling algorithm.
    for(i = 0; i<NO_OF_JOBS; i++)
    {
        avg_response_FIFO += FIFO_queue[i].response_time;
        avg_turnaround_FIFO += FIFO_queue[i].turnaround;
        total_context_switches_FIFO += FIFO_queue[i].context_switch;

        avg_response_SJF += SJF_queue[i].response_time;
        avg_turnaround_SJF += SJF_queue[i].turnaround;
        total_context_switches_SJF += SJF_queue[i].context_switch;

        avg_response_STCF += STCF_queue[i].response_time;
        avg_turnaround_STCF += STCF_queue[i].turnaround;
        total_context_switches_STCF += STCF_queue[i].context_switch;

        avg_response_RR1 += RR1_queue[i].response_time;
        avg_turnaround_RR1 += RR1_queue[i].turnaround;
        total_context_switches_RR1 += RR1_queue[i].context_switch;

        avg_response_RR2 += RR2_queue[i].response_time;
        avg_turnaround_RR2 += RR2_queue[i].turnaround;
        total_context_switches_RR2 += RR2_queue[i].context_switch;
    }

    avg_response_FIFO = avg_response_FIFO / NO_OF_JOBS;
    avg_turnaround_FIFO = avg_turnaround_FIFO / NO_OF_JOBS;

    avg_response_SJF = avg_response_SJF / NO_OF_JOBS;
    avg_turnaround_SJF = avg_turnaround_SJF / NO_OF_JOBS;

    avg_response_STCF = avg_response_STCF / NO_OF_JOBS;
    avg_turnaround_STCF = avg_turnaround_STCF / NO_OF_JOBS;

    avg_response_RR1 = avg_response_RR1 / NO_OF_JOBS;
    avg_turnaround_RR1 = avg_turnaround_RR1 / NO_OF_JOBS;

    avg_response_RR2 = avg_response_RR2 / NO_OF_JOBS;
    avg_turnaround_RR2 = avg_turnaround_RR2 / NO_OF_JOBS;

    // Writes job details to the output file in csv format.
    fprintf(fout, ",Job Details,,,,,,FIFO,,,,,,,,SJF,,,,,,,,STCF(30),,,,,,,,RR(30),,,,,,,,RR(75)\n");
    fprintf(fout, ",,, ,Avg Repsonse:,%.2f,Avg Turnaround:,%.2f,Total Context Switches:,%d,, ,Avg Repsonse:,%.2f,Avg Turnaround:,%.2f,Total Context Switches:,%d,, ,Avg Repsonse:,%.2f,Avg Turnaround:,%.2f,Total Context Switches:,%d,,, ,Avg Repsonse:,%.2f,Avg Turnaround:,%.2f,Total Context Switches:,%d,, ,Avg Repsonse:,%.2f,Avg Turnaround:,%.2f,Total Context Switches:,%d\n",
    avg_response_FIFO, avg_turnaround_FIFO, total_context_switches_FIFO, avg_response_SJF,avg_turnaround_SJF, total_context_switches_SJF,
    avg_response_STCF, avg_turnaround_STCF, total_context_switches_STCF, avg_response_RR1,avg_turnaround_RR1, total_context_switches_RR1,
    avg_response_RR2, avg_turnaround_RR2, total_context_switches_RR2);
    fprintf(fout,"Job No,Arrival Time,Job Size, ,Start Time,Time Left,Status,Context Switches,End Time,Response Time,Turnaround Time, ,Start Time,Time Left,Status,Context Switches,End Time,Response Time,Turnaround Time, ,Start Time,Time Left,Status,Context Switches,End Time,Response Time,Turnaround Time, ,Start Time,Time Left,Status,Context Switches,End Time,Response Time,Turnaround Time, ,Start Time,Time Left,Status,Context Switches,End Time,Response Time,Turnaround Time\n");

    for(i = 0; i < NO_OF_JOBS; i++)
    {
        fprintf(fout,"%d,%.2f,%.2f,,%.2f,%.2f,%d,%d,%.2f,%.2f,%.2f,,%.2f,%.2f,%d,%d,%.2f,%.2f,%.2f,,%.2f,%.2f,%d,%d,%.2f,%.2f,%.2f,,%.2f,%.2f,%d,%d,%.2f,%.2f,%.2f,,%.2f,%.2f,%d,%d,%.2f,%.2f,%.2f\n",
        FIFO_queue[i].job_no, FIFO_queue[i].arrival_time, FIFO_queue[i].size,
        FIFO_queue[i].start_time, FIFO_queue[i].time_left, FIFO_queue[i].status, FIFO_queue[i].context_switch, FIFO_queue[i].end_time, FIFO_queue[i].response_time, FIFO_queue[i].turnaround,
        SJF_queue[i].start_time, SJF_queue[i].time_left, FIFO_queue[i].status, SJF_queue[i].context_switch, SJF_queue[i].end_time, SJF_queue[i].response_time, SJF_queue[i].turnaround,
        STCF_queue[i].start_time, STCF_queue[i].time_left, FIFO_queue[i].status, STCF_queue[i].context_switch, STCF_queue[i].end_time, STCF_queue[i].response_time, STCF_queue[i].turnaround,
        RR1_queue[i].start_time, RR1_queue[i].time_left, FIFO_queue[i].status, RR1_queue[i].context_switch, RR1_queue[i].end_time, RR1_queue[i].response_time, RR1_queue[i].turnaround,
        RR2_queue[i].start_time, RR2_queue[i].time_left, FIFO_queue[i].status, RR2_queue[i].context_switch, RR2_queue[i].end_time, RR2_queue[i].response_time, RR2_queue[i].turnaround);
    }

    // Resets the arrays.
    memset(FIFO_queue, 0, sizeof(FIFO_queue));
    memset(SJF_queue, 0, sizeof(SJF_queue));
    memset(STCF_queue, 0, sizeof(STCF_queue));
    memset(RR1_queue, 0, sizeof(RR1_queue));
    memset(RR2_queue, 0, sizeof(RR2_queue));
}



/***************************************************************************************************
 * Function Name: FIFO()
 * Arguments: queue - An array of type structure Job used to contains job details.
 *            fin - File containing job arrival time and job size.
 * Description: Function to impletement First In First Out (FIFO) scheduling algorithm. 
 * *************************************************************************************************/

void FIFO(struct Job *queue, FILE *fin)
{
    fseek(fin, 0, SEEK_SET);        // Sets the cursor to the beginning of the file.

    int arrived = 0;                // Number of jobs that have arrived.
    int current;                    // Keeps track of the job being run.
    int time_clock = 0;             // Time clock variable used to keep track of time.

    // Loop to read the job runtime and arrival time and populates the array.
    for(current = 0; current < NO_OF_JOBS; current++)
    {
        fscanf(fin, "%f,%f", &queue[current].size, &queue[current].arrival_time);
        queue[current].job_no = current;
        queue[current].status = 0;
        queue[current].context_switch = 0;
        queue[current].time_left = queue[current].size;
    }

    current = 0;

    // Loops until all jobs are completed.
    while(current < NO_OF_JOBS)
    {   
        // Loop to check which jobs have arrived. If the arrival time of the job in the array at the index 'arrived' is less
        // than or equal to the time clock, then the status of the job is set to 1 and the variable 'arrived' is incremented.
        while (arrived < NO_OF_JOBS && queue[arrived].status == 0 && queue[arrived].arrival_time <= time_clock)
        {
            queue[arrived].status = 1;
            arrived++;
        }

        // If the job in the array at the index current has arrived.
        if(queue[current].status == 1)
        {
            // If the job in the array at the index current has started running for the first time.
            if(queue[current].time_left == queue[current].size)
            {
                queue[current].start_time = time_clock;
                queue[current].context_switch++;
                queue[current].response_time =  queue[current].start_time - queue[current].arrival_time;
            }
            
            // If the job in the array at the index current is completed, that is, time left is 0.
            if(queue[current].time_left <= 0)
            {
                queue[current].time_left = 0.00;
                queue[current].end_time = time_clock;
                queue[current].status = 2;
                queue[current].turnaround = queue[current].end_time - queue[current].arrival_time;

                current++;    // Next job
            }

            // If the job is running.
            else {
                queue[current].time_left--;     // Decrements current job's time left.
            }
        }
        time_clock++;   // Increments time.
    }
}



/***************************************************************************************************
 * Function Name: SJF()
 * Arguments: queue - An array of type structure Job used to contains job details.
 *            fin - File containing job arrival time and job size.
 * Description: Function to impletement Shortest Job First (SJF) scheduling algorithm. 
 * *************************************************************************************************/

void SJF(struct Job *queue, FILE *fin)
{
    fseek(fin, 0, SEEK_SET);        // Sets the cursor to the beginning of the file.

    int arrived = 0;                // Number of jobs that have arrived.
    int completed = 0;              // Keeps track of the number of jobs completed.
    int current;                    // Keeps track of the job being run.
    int pending_start = 0;          // Keeps track of which index of the 'ready' array to start sorting from. 
    int time_clock = 0;             // Time clock variable used to keep track of time.

    // Loop variables.
    int i = 0;
    int j = 0;

    int ready[NO_OF_JOBS];          // Array to store the job number of the jobs that have arrived.

    // Initializes the 'ready' array with -1.
    for(current = 0; current < NO_OF_JOBS; current++) {
        ready[current] = -1;
    }

    // Loop to read the job runtime and arrival time and populate the array.
    for(current = 0; current < NO_OF_JOBS; current++)
    {
        fscanf(fin, "%f,%f", &queue[current].size, &queue[current].arrival_time);
        queue[current].job_no = current;
        queue[current].status = 0;
        queue[current].context_switch = 0;
        queue[current].time_left = queue[current].size;
    }

    current = 0;

    // Loops until all jobs are completed.
    while(completed < NO_OF_JOBS)
    {   
        // Loop to check which jobs have arrived. If the arrival time of the job in the array at the index 'arrived' is less
        // than or equal to the time clock, then the status of the job is set to 1 and then the program sorts the 'ready'
        // array in increasing order of job sizes and places the currently arrived job at the appropriate index.
        // The variable 'arrived' is then incremented.
        while (arrived < NO_OF_JOBS && queue[arrived].status == 0 && queue[arrived].arrival_time <= time_clock)
        {
            queue[arrived].status = 1;

            // If no other jobs have arrived.
            if(arrived == 0) {
                ready[0] = arrived;
            }

            else{
                // Finds the appropriate index and shifts the 'ready' array by one to the right and places the arrived job
                // at the appropriate index. 
                for(i = pending_start; i < arrived; i++)
                {
                    if(queue[arrived].size < queue[ready[i]].size) {
                        for(j = arrived; j > i; j--) {
                            ready[j] = ready[j-1];
                        }
                        break;
                    }
                }
            }    
            ready[i] = arrived;        
            arrived++;
        }

        // If the job is running.
        if(ready[current] >= 0 && queue[ready[current]].status == 1)
        {
            // If the job has started running for the first time.
            if(queue[ready[current]].time_left == queue[ready[current]].size)
            {
                queue[ready[current]].start_time = time_clock;
                queue[ready[current]].context_switch = queue[ready[current]].context_switch + 1;
                queue[ready[current]].response_time =  queue[ready[current]].start_time - queue[ready[current]].arrival_time;
                pending_start++;    // Increments so the program does not sort the currently running job.
            }
            
            // If the job has ended.
            if(queue[ready[current]].time_left <= 0)
            {
                queue[ready[current]].time_left = 0.0;
                queue[ready[current]].end_time = time_clock;
                queue[ready[current]].status = 2;
                queue[ready[current]].turnaround = queue[ready[current]].end_time - queue[ready[current]].arrival_time;

                completed++;
                current++;    // Next job.
            }

            // If the job is running.
            else {
                queue[ready[current]].time_left--;  // Decrements current job's time left.
            }
        }
        time_clock++;   // Increments time.
    }
}



/****************************************************************************************************
 * Function Name: STCF()
 * Arguments: queue - An array of type structure Job used to contains job details.
 *            fin - File containing job arrival time and job size.
*             pre_emption_size - Integer variable that specifies the length of the time slice.
 * Description: Function to impletement Shortest Time-to-Completion First (STCF) scheduling algorithm. 
 * **************************************************************************************************/

void STCF(struct Job *queue, FILE *fin, int pre_emption_size)
{
    fseek(fin, 0, SEEK_SET);        // Sets the cursor to the beginning of the file.

    int arrived = 0;                        // Number of jobs that have arrived.
    int completed = 0;                      // Keeps track of the number of jobs completed.
    int current;                            // Keeps track of the job being run.
    int currently_over = 0;                 // Keeps track of the job whose time slice currently ended.
    int temp = 0;                           // Used for swap.
    int time_clock = 0;                     // Time clock variable used to keep track of time.
    int time_slice = pre_emption_size;      // Time variable use to keep track of time left in the time slice.

    // Loop variables.
    int i = 0;
    int j = 0;

    int ready[NO_OF_JOBS];                  // Array to store the job number of the jobs that have arrived.

    // Initializes the 'ready' array with -1.
    for(current = 0; current < NO_OF_JOBS; current++) {
        ready[current] = -1;
    }

    // Loop to read the job runtime and arrival time and populate the array.
    for(current = 0; current < NO_OF_JOBS; current++)
    {
        fscanf(fin, "%f,%f", &queue[current].size, &queue[current].arrival_time);
        queue[current].job_no = current;
        queue[current].status = 0;
        queue[current].context_switch = 0;
        queue[current].time_left = queue[current].size;
    }

    current = 0;

    // Loops until all jobs are completed.
    while(completed < NO_OF_JOBS)
    {   
        // Loop to check which jobs have arrived. If the arrival time of the job in the array at the index 'arrived' is less
        // than or equal to the time clock, then the status of the job is set to 1 and then the program sorts the 'ready'
        // array in increasing order of job sizes and places the currently arrived job at the appropriate index.
        // The variable 'arrived' is then incremented.
        while (arrived < NO_OF_JOBS && queue[arrived].status == 0 && queue[arrived].arrival_time <= time_clock)
        {
            queue[arrived].status = 1;

            // If no other jobs have arrived.
            if(arrived == 0) {
                ready[0] = arrived;
            }

            else
            {
                // Finds the appropriate index and shifts the 'ready' array by one to the right and places the arrived job
                // at the appropriate index.
                for(i = 0; i < arrived; i++)
                {
                    if(queue[arrived].size < queue[ready[i]].size) {
                        for(j = arrived; j > i; j--) {
                            ready[j] = ready[j-1];
                        }

                        // Updates current to account for the shift if current was affected.
                        if(current >= i) {
                            current++;
                        }
                        break;
                    }
                }
            }    
            ready[i] = arrived;        
            arrived++;
        }

        // If the job is running.
        if(ready[current] >= 0 && queue[ready[current]].status == 1)
        {
            // If the job has started running for the first time.
            if(queue[ready[current]].time_left == queue[ready[current]].size)
            {
                queue[ready[current]].start_time = time_clock;
                queue[ready[current]].context_switch = queue[ready[current]].context_switch + 1;
                queue[ready[current]].response_time =  queue[ready[current]].start_time - queue[ready[current]].arrival_time;
            }
            
            // If the job has ended.
            if(queue[ready[current]].time_left <= 0)
            {
                queue[ready[current]].time_left = 0;
                queue[ready[current]].end_time = time_clock;
                queue[ready[current]].status = 2;
                queue[ready[current]].turnaround = queue[ready[current]].end_time - queue[ready[current]].arrival_time;

                completed++;
                current++;                      // Next job
                time_slice = pre_emption_size;  // Resets time slice.
            }

            // If the job is running.
            else
            {
                queue[ready[current]].time_left--;  // Decrements current job's time left.
                time_slice--;                       // Decrements time slice.
            }

             // If the time slice is over.
            if(time_slice == 0)
            {
                time_slice = pre_emption_size;  // Resets time slice.
                currently_over = current;       // Stores 'current' value in 'currently_over'.

                // Loop to sort 'ready' array in increasing order of each of the arrived job's time left.
                for(i = 0; i < arrived; i++)
                {   
                    // Swaps indices if the time left of the current job is greater than the time left of the other arrived job.
                    if(queue[ready[i]].status != 2 && queue[ready[currently_over]].time_left > queue[ready[i]].time_left) 
                    {
                        temp = i;
                        i = current;
                        current = temp;
                    }
                }
                current = 0;    // Sets 'current' to the beginning of the array.

                // Loop to find the next job that has arrived and has not been completed.
                while(queue[ready[current]].status != 1)
                {
                    current++;
                    if(current >= arrived)  // Exits loop if none could be found.
                    {
                        current = 0;
                        break;
                    }
                }

                // If a context switch happens.
                if(queue[ready[currently_over]].job_no != queue[ready[current]].job_no)
                {
                    queue[ready[currently_over]].context_switch++;
                    queue[ready[current]].context_switch++;
                }
            }
        }

        // If the current job has already been completed, finds the next arrived that job that has not been completed.
        else if(queue[ready[current]].status == 2)
        {

            // Loop to find the next job that has arrived and has not been completed.
            while(queue[ready[current]].status != 1)
            {
                current++;

                if(current >= arrived)  // Exits loop if none could be found.
                {
                    current = 0;
                    break;
                }
            }
        }  
        time_clock++;   // Increments time.
    }

}



/****************************************************************************************************
 * Function Name: RR()
 * Arguments: queue - An array of type structure Job used to contains job details.
 *            fin - File containing job arrival time and job size.
 *            quantum_size - Integer variable that specifies the length of the time slice.
 * Description: Function to impletement Round Robin (RR) scheduling algorithm. 
 * **************************************************************************************************/

void RR(struct Job *queue, FILE *fin, int quantum_size)
{
    fseek(fin, 0, SEEK_SET);        // Sets the cursor to the beginning of the file.

    int arrived = 0;                    // Number of jobs that have arrived.
    int completed = 0;                  // Keeps track of the number of jobs completed.
    int current;                        // Keeps track of the job being run.
    int currently_over = 0;             // Keeps track of the job whose time slice currently ended.
    int time_clock = 0;                 // Time clock variable used to keep track of time.
    int time_slice = quantum_size;      // Time variable use to keep track of time left in the time slice.


    int ready[NO_OF_JOBS];              // Array to store the job number of the jobs that have arrived.

    // Initializes the 'ready' array with -1.
    for(current = 0; current < NO_OF_JOBS; current++) {
        ready[current] = -1;
    }

    // Loop to read the job runtime and arrival time and populate the array.
    for(current = 0; current < NO_OF_JOBS; current++)
    {
        fscanf(fin, "%f,%f", &queue[current].size, &queue[current].arrival_time);
        queue[current].job_no = current;
        queue[current].status = 0;
        queue[current].context_switch = 0;
        queue[current].time_left = queue[current].size;
    }

    current = 0;

    // Loops until all jobs are completed.
    while(completed < NO_OF_JOBS)
    {   
        // Loop to check which jobs have arrived. If the arrival time of the job in the array at the index 'arrived' is less
        // than or equal to the time clock, then the status of the job is set to 1 and the value at the index 'arrived' of 
        // the 'ready' array is set to 'arrived'. The variable 'arrived' is then incremented.
        while (arrived < NO_OF_JOBS && queue[arrived].status == 0 && queue[arrived].arrival_time <= time_clock)
        {
            queue[arrived].status = 1;
            ready[arrived] = arrived;        
            arrived++;
        }

        // If the job is running.
        if(ready[current] >= 0 && queue[ready[current]].status == 1)
        {
            // If the job has started running for the first time.
            if(queue[ready[current]].time_left == queue[ready[current]].size)
            {
                queue[ready[current]].start_time = time_clock;
                queue[ready[current]].context_switch = queue[ready[current]].context_switch + 1;
                queue[ready[current]].response_time =  queue[ready[current]].start_time - queue[ready[current]].arrival_time;
            }
            
            // If the job has ended.
            if(queue[ready[current]].time_left <= 0)
            {
                queue[ready[current]].time_left = 0.0;
                queue[ready[current]].end_time = time_clock;
                queue[ready[current]].status = 2;
                queue[ready[current]].turnaround = queue[ready[current]].end_time - queue[ready[current]].arrival_time;
                
                completed++;
                current++;                  // Next job.
                time_slice = quantum_size;  // Resets time slice.

            }

            // If the job is running.
            else
            {
                queue[ready[current]].time_left--;  // Decrements current job's time left.
                time_slice--;                       // Decrements time slice.
            }

            // If the time slice is over.
            if(time_slice == 0)
            {
                currently_over = current;   // Stores 'current' value in 'currently_over'.
                time_slice = quantum_size;  // Resets time slice.
                current++;

                // If 'current' exceeds the number of arrived jobs, then sets 'current' to the beginning of the array.
                if(current >= arrived) {
                    current = 0;
                }

                // Loop to find the next job that has arrived and has not been completed.
                while(queue[ready[current]].status != 1)
                {
                    current++;
                    
                    // Exits loop if none could be found.
                    if(current == currently_over) {
                        break;
                    }
                    // If 'current' exceeds the number of arrived jobs, then sets 'current' to the beginning of the array.
                    if(current >= arrived) {
                        current = 0;
                    }
                }

                // If a context switch happens.
                if(current != currently_over)
                {
                    queue[ready[currently_over]].context_switch++;
                    queue[ready[current]].context_switch++;
                }
            }
        }

        // If the current job has already been completed, finds the next arrived that job that has not been completed.
        else if(queue[ready[current]].status == 2)
        {
            current++;

            // If 'current' exceeds the number of arrived jobs, then sets 'current' to the beginning of the array.
            if(current >= arrived) {
                current = 0;
            }
            currently_over = current;

            // Loop to find the next job that has arrived and has not been completed.
            while(queue[ready[current]].status != 1)
            {
                current++;

                // Exits loop if none could be found.
                if(current == currently_over) {
                    break;
                }
                // If 'current' exceeds the number of arrived jobs, then sets 'current' to the beginning of the array.           
                if(current >= arrived) {
                    current = 0;
                }
            }
        }     
        time_clock++;   // Increments time.
    }
}
