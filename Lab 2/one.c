#include <stdio.h>
int main() 
{
    int n, i, j, total_time, waiting_time = 0, turnaround_time = 0;
    int burst_time[20], arrival_time[20], finish_time[20], waiting_time_arr[20], turnaround_time_arr[20];

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        printf("Enter the burst time for process %d: ", i + 1);
        scanf("%d", &burst_time[i]);
        printf("Enter the arrival time for process %d: ", i + 1);
        scanf("%d", &arrival_time[i]);
    }
    // Calculate finish time for each process
    total_time = arrival_time[0];
    for (i = 0; i < n; i++) {
        // Add gap time between processes
        if (arrival_time[i] > total_time) {
            total_time = arrival_time[i];
        }
        finish_time[i] = total_time + burst_time[i];
        total_time = finish_time[i];
    }
    // Calculate waiting time and turnaround time for each process
    for (i = 0; i < n; i++) {
        waiting_time_arr[i] = finish_time[i] - burst_time[i] - arrival_time[i];
        turnaround_time_arr[i] = finish_time[i] - arrival_time[i];

        waiting_time += waiting_time_arr[i];
        turnaround_time += turnaround_time_arr[i];
    }
    // Print results
    printf("\nProcess\tBurst Time\tArrival Time\tFinish Time\tWaiting Time\tTurnaround Time\n");
    for (i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", i + 1, burst_time[i], arrival_time[i], finish_time[i], waiting_time_arr[i], turnaround_time_arr[i]);
    }
    printf("Average waiting time: %.2f\n", (float) waiting_time / n);
    printf("Average turnaround time: %.2f\n", (float) turnaround_time / n);
    return 0;
}
