#include <stdio.h>
#include <stdlib.h>
#include "process.h"

// Comparison function for Priority scheduling
int compare_priority(const void *a, const void *b) {
    ProcessType *pA = (ProcessType *)a;
    ProcessType *pB = (ProcessType *)b;
    return pA->pri - pB->pri;  // Sort processes by priority (ascending)
}

// Comparison function for Shortest Job First (SJF) scheduling
int compare_burst_time(const void *a, const void *b) {
    ProcessType *pA = (ProcessType *)a;
    ProcessType *pB = (ProcessType *)b;
    return pA->bt - pB->bt;  // Sort processes by burst time (ascending)
}

// Priority Scheduling Implementation
void priority_scheduling(ProcessType processes[], int count) {
    qsort(processes, count, sizeof(ProcessType), compare_priority);  // Sort processes by priority

    int accumulated_wait = 0;  // Tracks total waiting time
    for (int i = 0; i < count; i++) {
        processes[i].wt = accumulated_wait;  // Assign waiting time to process
        processes[i].tat = processes[i].wt + processes[i].bt;  // Calculate turnaround time
        accumulated_wait += processes[i].bt;  // Update total waiting time
    }
}

// Shortest Job First (SJF) Scheduling Implementation
void shortest_job_first(ProcessType processes[], int count) {
    qsort(processes, count, sizeof(ProcessType), compare_burst_time);  // Sort processes by burst time

    int accumulated_wait = 0;  // Tracks total waiting time
    for (int i = 0; i < count; i++) {
        processes[i].wt = accumulated_wait;  // Assign waiting time to process
        processes[i].tat = processes[i].wt + processes[i].bt;  // Calculate turnaround time
        accumulated_wait += processes[i].bt;  // Update total waiting time
    }
}

// Round Robin Scheduling Implementation
void round_robin(ProcessType processes[], int count, int time_quantum) {
    int *remaining_bt = (int *)malloc(count * sizeof(int));  // Array for remaining burst times

    for (int i = 0; i < count; i++) {
        remaining_bt[i] = processes[i].bt;  // Initialize remaining burst times
    }

    int current_time = 0;  // Tracks the current time in the simulation
    int completed_processes = 0;  // Tracks how many processes are fully executed

    while (completed_processes < count) {
        for (int i = 0; i < count; i++) {
            if (remaining_bt[i] > 0) {  // Process is not yet completed
                if (remaining_bt[i] > time_quantum) {
                    current_time += time_quantum;
                    remaining_bt[i] -= time_quantum;
                } else {
                    current_time += remaining_bt[i];
                    processes[i].wt = current_time - processes[i].bt;
                    remaining_bt[i] = 0;
                    completed_processes++;
                }
            }
        }
    }

    for (int i = 0; i < count; i++) {
        processes[i].tat = processes[i].wt + processes[i].bt;  // Calculate turnaround time
    }

    free(remaining_bt);
}

// Helper function to print process details
void print_processes(ProcessType processes[], int count) {
    printf("PID\tBurst Time\tArrival Time\tPriority\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", 
               processes[i].pid, processes[i].bt, processes[i].art, 
               processes[i].pri, processes[i].wt, processes[i].tat);
    }
}

// Main function
int main() {
    int num_processes, time_quantum;

    printf("Enter the number of processes: ");
    scanf("%d", &num_processes);

    ProcessType *process_list = (ProcessType *)malloc(num_processes * sizeof(ProcessType));
    if (!process_list) {
        printf("Error: Unable to allocate memory for process list.\n");
        return EXIT_FAILURE;
    }

    printf("Enter time quantum for Round Robin scheduling: ");
    scanf("%d", &time_quantum);

    for (int i = 0; i < num_processes; i++) {
        process_list[i].pid = i + 1;  // Assign process ID
        printf("Enter burst time and priority for process %d: ", process_list[i].pid);
        scanf("%d %d", &process_list[i].bt, &process_list[i].pri);
        process_list[i].art = 0;  // Default arrival time is 0
    }

    printf("\nPriority Scheduling\n");
    priority_scheduling(process_list, num_processes);
    print_processes(process_list, num_processes);

    printf("\nShortest Job First (SJF) Scheduling\n");
    shortest_job_first(process_list, num_processes);
    print_processes(process_list, num_processes);

    printf("\nRound Robin Scheduling\n");
    round_robin(process_list, num_processes, time_quantum);
    print_processes(process_list, num_processes);

    free(process_list);  // Release allocated memory
    return 0;
}
