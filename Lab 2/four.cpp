#include <stdio.h>
#include <stdlib.h>
#define MAX_PROCESS 10
int main() {
    int num_processes, temp;
    int priority[MAX_PROCESS], burst_time[MAX_PROCESS], arrival_time[MAX_PROCESS], waiting_time[MAX_PROCESS], turnaround_time[MAX_PROCESS];
    int completion_time[MAX_PROCESS], remaining[MAX_PROCESS], min_priority_index, time, current_process;
    float avg_waiting_time = 0, avg_turnaround_time = 0;
    printf("Enter the number of processes: ");
    scanf("%d", &num_processes);
// Input arrival time, priority, and burst time for each process
    printf("Enter the arrival time, priority, and burst time for each process:\n");
    for (int i = 0; i < num_processes; i++) {
        printf("Process %d - Arrival Time: ", i+1);
        scanf("%d", &arrival_time[i]);
        printf("Process %d - Priority: ", i+1);
        scanf("%d", &priority[i]);
        printf("Process %d - Burst Time: ", i+1);
        scanf("%d", &burst_time[i]);
        remaining[i] = burst_time[i];
    }
    // Sort the processes based on arrival time using selection sort algorithm
    for (int i = 0; i < num_processes-1; i++) {
        min_priority_index = i;
        for (int j = i+1; j < num_processes; j++) {
            if (arrival_time[j] < arrival_time[min_priority_index]) {
                min_priority_index = j;
            }
        }
        // Swap arrival time, priority, and burst time values for the processes with minimum arrival time
        temp = arrival_time[i];
        arrival_time[i] = arrival_time[min_priority_index];
        arrival_time[min_priority_index] = temp;
        temp = priority[i];
        priority[i] = priority[min_priority_index];
        priority[min_priority_index] = temp;
        temp = burst_time[i];
        burst_time[i] = burst_time[min_priority_index];
        burst_time[min_priority_index] = temp;
    }
    time = 0;
    int done = 0;
    // Execute processes based on priority
    while (done < num_processes) {
        min_priority_index = -1;
        int min_priority = 1000;
        for (int i = 0; i < num_processes; i++) {
            if (remaining[i] > 0 && arrival_time[i] <= time && priority[i] < min_priority) {
                min_priority_index = i;
                min_priority = priority[i];
            }
        }
        if (min_priority_index == -1) {
            time++;
        }
        else {
            current_process = min_priority_index;
            if (remaining[current_process] > 1) {
                remaining[current_process]--;
            }
            else {
                remaining[current_process] = 0;
                done++;
                completion_time[current_process] = time + 1;
                waiting_time[current_process] = completion_time[current_process] - burst_time[current_process] - arrival_time[current_process];
                if (waiting_time[current_process] < 0) {
                    waiting_time[current_process] = 0;
                }
                turnaround_time[current_process] = completion_time[current_process] - arrival_time[current_process];
            }
            time++;
        }
    }
// Calculate average waiting time and average turnaround time
for (int i = 0; i < num_processes; i++) {
    avg_waiting_time += waiting_time[i];
    avg_turnaround_time += turnaround_time[i];
}
avg_waiting_time /= num_processes;
avg_turnaround_time /= num_processes;
// Output the results
printf("\n");
printf("Process\tPriority\tBurst Time\tArrival Time\tWaiting Time\tTurnaround Time\n");
for (int i = 0; i < num_processes; i++) {
    printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", i+1, priority[i], burst_time[i], arrival_time[i], waiting_time[i], turnaround_time[i]);
}
printf("\nAverage Waiting Time: %.2f\n", avg_waiting_time);
printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);
return 0;
}
